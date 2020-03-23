#include <sys/types.h>

struct Analyzer{
    char* lineAddress;
    int currentOffset;
    ssize_t lineSize;
};

struct Package{
    char* packageName;
    char* installDate;
    char* lastUpdate;
    char* removalDate;
    char isInstalled, isUpdated;
    int timesUpdated;
    struct Package* nextPkg;
};

struct PkgNode{
    struct Package* pkg;
    struct Package* next;
};

struct Report{
    struct Package* Table[2048];
    int installedPkgs, removedPkgs, upgradedPkgs, currentPkgs;
};

struct Analyzer* createAnalyzer();
struct Report* createReport();
struct Package* createPackage(char*, char*);

int analizeLog(char*, char*);
void getLineSize(struct Analyzer* , char*);
int getNameSize(char*,int);
char* getPackageName(char*,int);
char* getPackageDate(char*);
void update(struct Package*, char*, struct Report*);
void removeP(struct Package*, char*, struct Report*);
void printPackage(struct Package*, FILE*);
int hashCode(char*, ssize_t);
void add(struct Report*, char*, char*);
struct Package* searchPkg(struct Report*, char*);
void getReport(struct Report*, char*);



