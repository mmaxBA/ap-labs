#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mystrlen(char *str){
    int len = 0;
    for( len; str[len] != '\0'; len++);
    return len;
}

char *mystradd(char *origin, char *addition){
    int origin_len = mystrlen(origin);
    int add_len = mystrlen(addition);
    int new_len = origin_len +add_len;
    char* str = malloc(sizeof(char)*(new_len));
    for(int i = 0; i < origin_len; i++){
        str[i] = origin[i];
    }
    for (int j = origin_len; j <new_len ; ++j) {
        str[j] = addition[j-origin_len];
    }
    return str;
}

int mystrfind(char *origin, char *substr){
    int origin_len = mystrlen(origin);
    int sub_len = mystrlen(substr);
    if (sub_len > origin_len) {
        return 0;
    }
    int found = 0;

    for(int i = 0; i<origin_len-sub_len; i++){
        int z = 0;
        for(int j = i; j < i+sub_len; j++){
            if(origin[j] != substr[z]){
                found = 0;
                break;
            }
            z++;
            found = 1;
        }
        if(found){
            return 1;
        }
    }
    return 0;
}

