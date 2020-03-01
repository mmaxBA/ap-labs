#include <stdio.h>
#include <stdlib.h>

int main(){
    char c, tmp;
    char* word;
    int count = 0;
    word = malloc(4* sizeof(char));
    while (( c = getchar()) != EOF) {
        if(c == '\n'){
            for(int i = 0; i < count/2; i++){
                tmp = word[i];
                word[i] = word[count - i - 1];
                word[count - i - 1] = tmp;
            }

            printf("%s\n", word);
            count = 0;
            free(word);
            word = malloc(4* sizeof(char));
        }
        *(word + count) = c;
        count++;
    }
    free(word);
    return 0;
}
