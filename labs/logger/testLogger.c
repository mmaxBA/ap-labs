#include <stdio.h>
#include "logger.c"

void main(){
    int x = 1;
    char* y = "W";
    float z = 1.0f;
    infof("Info, valor: %d \n",x);
    warnf("Advertencia, valor: %s\n",y);
    errorf("Error, valor: %f \n",z);
    panicf("Panico\n");
}
