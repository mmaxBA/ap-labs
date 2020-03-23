#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include "header.h"

#define REPORT_FILE "packages_report.txt"
#define INSTALL_INDEX 36
#define UPGRADE_INDEX 35
#define  REMOVE_INDEX 34
#define SIZE 1500

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Please specify a file to analyze\n");
        return 1;
    }
    return analizeLog(argv[1], REPORT_FILE);;
}
int analizeLog(char* log, char* report)
{
    printf("Generating Report from: [%s] log file\n", log);
    //Abrir el archivo log, crear un analizador y un reporte(hashtable)
    int fileD = open(log, O_RDONLY);
    off_t fileSize = lseek(fileD, (size_t)0, SEEK_END);
    lseek(fileD, (size_t)0, SEEK_SET);

    struct Analyzer* analyzer = createAnalyzer();
    struct Report* finalReport = createReport();

    //Buffer donde se guarda todo el contenido del archivo y se inicializa con el tamaño del archivo
    char* fileBuffer = (char*)malloc(sizeof(char)*fileSize);
    analyzer->lineAddress = fileBuffer;

    // Se guarda el contenido del archivo en el buffer
    read(fileD,fileBuffer,fileSize);

    char* pkgName;
    char* pkgDate;
    struct Package* updatePtr;

    int i = 0;
    int y = 0;

    //Obtenemos el numero de lineas en el archivo
    unsigned int lineCounter = 0;
    while(i < fileSize){
        if(fileBuffer[i] == '\n'){
            lineCounter++;
        }
        i++;
    }

    //Iteramos por cada linea que hay
    while(y < lineCounter){
        //Obtenemos el tamaño de la linea para saber en donde es que termina
        getLineSize(analyzer, analyzer->lineAddress);
        //Obtenemos la line
        char* lineBuffer = (char*)malloc(sizeof(char)*analyzer->lineSize);
        for(int i = 0; i < analyzer->lineSize; i++){
            lineBuffer[i] = fileBuffer[analyzer->currentOffset++];
        }
        analyzer->currentOffset++;
        //Decidimos si el paquete se añade, actualiza o elimina
        if(strstr(lineBuffer,"[ALPM] installed")!= NULL){
            pkgName = getPackageName(lineBuffer,INSTALL_INDEX);
            pkgDate = getPackageDate(lineBuffer);
            add(finalReport, pkgName, pkgDate);
        }
        else if(strstr(lineBuffer,"[ALPM] upgraded")!= NULL){
            updatePtr = searchPkg(finalReport,getPackageName(lineBuffer,UPGRADE_INDEX));
            pkgDate = getPackageDate(lineBuffer);
            update(updatePtr,pkgDate, finalReport);
        }
        else if(strstr(lineBuffer,"[ALPM] removed")!= NULL){
            updatePtr = searchPkg(finalReport,getPackageName(lineBuffer,REMOVE_INDEX));
            pkgDate = getPackageDate(lineBuffer);
            removeP(updatePtr,pkgDate, finalReport);
        }
        y++;
    }
    //Generar reporte
    getReport(finalReport,report);

    //Liberar el espacio
    free(fileBuffer);
    free(analyzer);
    free(finalReport);

    printf("Report is generated at: [%s]\n", report);
    return close(fileD);
}
//Struct creation methods

struct Analyzer* createAnalyzer(){
    struct Analyzer* newAnalyzer = (struct Analyzer*)malloc(sizeof(struct Analyzer));
    newAnalyzer->currentOffset = 0;
    return newAnalyzer;
}

struct Package* createPackage(char* name, char* date){
    struct Package* newPkg = (struct Package*)malloc(sizeof(struct Package));
    newPkg->packageName = name;
    newPkg->installDate = date;
    newPkg->isInstalled = 1;
    newPkg->isUpdated = 0;
    newPkg->timesUpdated = 0;
    return newPkg;
}

struct Report* createReport(){
    struct Report* newReport = (struct Report*)malloc(sizeof(struct Report));
    newReport->currentPkgs = 0;
    newReport->installedPkgs = 0;
    newReport->removedPkgs = 0;
    newReport->upgradedPkgs = 0;
    return newReport;
}


void getLineSize(struct Analyzer* analyzer, char* buffer){
    ssize_t lineSize = 0;
    do{
        lineSize++;
        buffer+=1;
    }
    while(*buffer!= '\n');
    analyzer->lineAddress = buffer + 1;
    analyzer->lineSize = lineSize;
}

int getNameSize(char* lineBuffer, int offset){
    int nameSize = 0;
    while(lineBuffer[offset]!=' '){
        nameSize++;
        offset++;
    }
    return nameSize;
}

char* getPackageName(char* lineBuffer, int offset){
    int size = getNameSize(lineBuffer,offset);
    char* name = (char*)malloc(sizeof(char)*size);
    char i = offset;
    char j = 0;
    while(lineBuffer[i]!=' '){
        name[j++] = lineBuffer[i];
        i++;
    }
    return name;
}

char* getPackageDate(char* lineBuffer){
    char* date = (char*)malloc(sizeof(char)*16);
    for(int i = 0; i < 18;i++){
        date[i] = lineBuffer[i];
    }
    return date;
}

void update(struct Package* pkgName, char* date, struct Report* fileReport){
    fileReport->upgradedPkgs += pkgName->isUpdated ? 1 : 0;
    pkgName->isUpdated = 1;
    pkgName->lastUpdate = date;
    pkgName->timesUpdated+=1;
}

void removeP(struct Package* pkgName, char* date, struct Report* fileReport){
    pkgName->removalDate=date;
    pkgName->isInstalled = 0;
    fileReport->removedPkgs++;
}

void printPackage(struct Package* pkg, FILE* fp){
    if(pkg->lastUpdate == NULL){
        pkg->lastUpdate = "-";
    }
    if(pkg->removalDate == NULL){
        pkg->removalDate = "-";
    }
    fprintf(fp,"- Package Name: %s\n", pkg->packageName);
    fprintf(fp,"Install date: %s\n",pkg->installDate);
    fprintf(fp,"Last Update: %s\n",pkg->lastUpdate);
    fprintf(fp, "Number of updates: %d\n", pkg->timesUpdated);
    fprintf(fp,"Removal Date: %s\n", pkg->removalDate);
}

int hashCode(char* name, ssize_t size){
    return (long int)*name % size;
}

void add(struct Report* report, char* name, char* date){
    struct Package* newPkg = createPackage(name,date);
    int hashIndex = hashCode(newPkg->packageName,SIZE);
    if(report->Table[hashIndex] == NULL){
        report->Table[hashIndex] = newPkg;
    }
    else{
        struct Package* i = report->Table[hashIndex];
        while(i!=NULL){
            if(i->nextPkg == NULL){
                i->nextPkg = newPkg;
                break;
            }
            i = i->nextPkg;
        }
    }
    report->installedPkgs++;
}

struct Package* searchPkg(struct Report* report, char* name){
    int hashIndex = hashCode(name,SIZE);
    struct Package* i = report->Table[hashIndex];
    while(i!=NULL){
        if(!strcmp(name,i->packageName)){
            return i;
        }
        i = i->nextPkg;
    }
    return 0;
}

void getReport(struct Report* report, char* logFile){
    FILE * fp;
    fp = fopen (logFile,"w");
    report->currentPkgs = (report->installedPkgs) - (report->removedPkgs);
    fprintf(fp,"Pacman Packages Report\n----------------------\n");
    fprintf(fp,"- Installed packages : %d\n", report->installedPkgs);
    fprintf(fp, "- Removed packages : %d\n", report->removedPkgs);
    fprintf(fp,"- Upgraded packages  : %d\n",  report->upgradedPkgs);
    fprintf(fp, "- Current installed  : %d\n",  report->currentPkgs);
    fprintf(fp,"-------------------------\n");
    for(int i = 0; i < SIZE; i++){
        if(report->Table[i]!=NULL){
            struct Package* temp = report->Table[i];
            while(temp!=NULL){
                printPackage(temp,fp);
                temp = temp->nextPkg;
            }
        }
    }
    fclose (fp);

}