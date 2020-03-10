#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void merge(void *arr, int l, int m, int r, size_t size, int (*comp)(void *, void *)){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    void* L = malloc(size*n1);
    void* R = malloc(size*n2);

    for (i = 0; i < n1; i++) {
        memcpy(L + size*i, arr+size*(l+i),size);
    }
    for (j = 0; j < n2; j++)
        memcpy(R+ size*j, arr+size*(m+1 + j),size);

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2){
        if (comp(L + size *i, R+ size*j)){;
            memcpy(arr + size*k, L+ size*i, size);
            i++;
        }else{
            memcpy(arr + size * k, R +size*j, size);
            j++;
        }
        k++;
    }
    while (i < n1){
        memcpy(arr+size*k, L+size*i, size);
        i++;
        k++;
    }
    while (j < n2){
        memcpy(arr + size * k, R +size*j, size);
        j++;
        k++;
    }
    free(L);
    free(R);
}

void mergeSort(void *arr, int l, int r, size_t size, int(*comp)(void *, void *)){
    if(l<r){
        int m = l+(r-l)/2;
        mergeSort(arr, l, m, size, comp);
        mergeSort(arr, m+1, r, size, comp);
        merge(arr, l, m, r, size , comp);
    }
}

int intcmp(void* a, void* b){
    int *x = (int*) a;
    int *y = (int*)b;
    return *x <= *y ? 1 : 0;
}

int charcmp(void* a, void* b){
    char *x = (char*) a;
    char *y = (char*)b;
    return *x <= *y ? 1 : 0;
}

int floatcmp(void* a, void* b){
    float *x = (float*) a;
    float *y = (float*)b;
    return *x <= *y ? 1 : 0;
}

void printFloat(float* arr, char size){
    for(char i = 0; i < size; i++){
        printf("%.6f,",arr[i]);
    }
    printf("\n");
}
void printChar(char* arr, char size){
    for(char i = 0; i < size; i++){
        printf("%c,",arr[i]);
    }
    printf("\n");
}
void printInt(int* arr, char size){
    for(char i = 0; i < size; i++){
        printf("%d,",arr[i]);
    }
    printf("\n");
}

int main()
{
    float a[] = {3.0f, 6.0f, 2.0f, 2.2f, 9.0f};
    printFloat(a, sizeof(a)/sizeof(float));
    mergeSort(a, 0, 3, sizeof(float), floatcmp);
    printFloat(a, sizeof(a)/sizeof(float));

    int b[] = {5,6,7,8,2};
    printInt(b, sizeof(b)/sizeof(int));
    mergeSort(b, 0, 4, sizeof(int), intcmp);
    printInt(b, sizeof(b)/sizeof(int));

    char c[] = {'z', 'x', 'a', 'b'};
    printChar(c, sizeof(c)/sizeof(char));
    mergeSort(c, 0, 3, sizeof(char), charcmp);
    printChar(c, sizeof(c)/sizeof(char));
    return 0;
}
