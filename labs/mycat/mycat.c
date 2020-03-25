#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Missing parameters\n");
        return -1;
    }
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1){
        printf("Invalid file\n");
        return -1;
    }
    off_t curOffset = lseek(fd, (size_t)0, SEEK_CUR);
    off_t size = lseek(fd, (size_t)0, SEEK_END);
    lseek(fd, curOffset, SEEK_SET);

    char* fileBuffer = (char*)malloc(sizeof(char)*size);

    read(fd,fileBuffer,size);
    write(1,fileBuffer,size);
    close(fd);
    free(fileBuffer);
    return 0;
}