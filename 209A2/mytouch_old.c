#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char **argv)
{
    int fd;
    int i,status,byte;
    char data[1];
    status = 0;
    if (argc <= 1){
        fprintf(stderr, "usage: mytouch file ...\n");
        return(1);
    }
    for (i = 1; i < argc; i ++){
        fd = open(argv[i], O_RDWR|O_CREAT, 0666);
        if (fd == -1){
            perror(argv[i]);
            status = 1;
        }else{
            byte = read(fd,data,1);
            if (byte == EOF){
                write(fd,"",1);
                ftruncate(fd,0);
            }else{
                lseek(fd,0,SEEK_SET);
                write(fd,data,1);
            }
        }
        close(fd);
    }
    
    return(status);
}