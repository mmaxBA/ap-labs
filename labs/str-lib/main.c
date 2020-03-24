#include <stdio.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char *argv[]) {

    if(argc < 4){
        printf("You need to pass 3 arguments\n");
        return -1;
    }

    char* originalStr = argv[1];
    char* addStr = argv[2];
    char* subStr = argv[3];
    char* newStr = mystradd(originalStr, addStr);
    char* isSub = mystrfind(originalStr, subStr) ? "yes" : "no";

    printf("Initial Lenght     : %d\n", mystrlen(originalStr));
    printf("New String         : %s\n", newStr);
    printf("SubString was found: %s\n", isSub);

    return 0;
}