#include <stdio.h>
#include "logger.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define BUFFER_LEN sizeof(struct inotify_event) * 1024

int fd;
int wd;
int rd;
char* p;
char* name;
struct inotify_event *event;

void printEvent(struct inotify_event* event);

int main(char argc, char** argv){
    if(argc < 2){
        printf("Especifica un directorio\n");
        return -1;
    }
    fd = inotify_init1(O_NONBLOCK);
    wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
    name = argv[1];

    char* buffer = (char*)malloc(BUFFER_LEN);
    while(1){
        rd = read(fd, buffer, BUFFER_LEN);
        p = buffer;
        event = (struct inotify_event*)p;
        for (p = buffer; p < buffer + rd; ) {
            event = (struct inotify_event *) p;
            printEvent(event);
            p += sizeof(struct inotify_event) + event->len;
        }
    }
    close(fd);
    return 0;
}

void printEvent(struct inotify_event* event){
    if (event->mask & IN_ACCESS)    infof("%s accesado\n", name);
    if (event->mask & IN_CREATE)        warnf("Subdirectorio o archivo creado\n");
    if (event->mask & IN_DELETE)        warnf("Subdirectorio o archivo eliminado\n");
    if (event->mask & IN_OPEN)          infof("%s  abierto\n", name);
    if (event->mask & IN_MODIFY)          warnf("Modificacion en archivo del directorio\n");
}