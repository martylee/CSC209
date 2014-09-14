#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 1024

int fd[MAX_LEN][2];

int main(int argc, char **argv){
    int i;
    extern int process0(int n,int num);
    if (argc != 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    int num = atoi(argv[1]);
    if (num < 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    
    int ppid,pid,status,result;
    
    
    for (i = 0;i < (num -1); i ++){
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }
    }
    
    process0(0,num);
    return 0;
}

int process0(int n,int num){
    int result;
    char readbuf[MAX_LEN];
    extern int process(char *argv,int num);
    if (n == 0){
        while (fgets(readbuf, MAX_LEN, stdin)) {
            if (close(fd[0][0]) != 0) {
                
                perror("close");
                exit(1);
            }
            if ((result = write(fd[0][1],readbuf,sizeof(readbuf))) < 0){
                perror("write");
                exit(1);
            }
            printf("process #0 (%d) sending message: %s",getpid(),readbuf);
            process(readbuf,num);

        
        }
    }else if (n == (num - 1)){
        if (close(fd[n][1]) != 0) {
            
            perror("close");
            exit(1);
        }
        if ((result = read(fd[n][0],readbuf,sizeof(readbuf))) > 0){
            printf("I hear %s",readbuf);
            process(0,num);
        }
    }
    return 0;
}

int process(char *readbuf,int num){
    int i;
    int result;
    for (i = 1; i < num - 1; i++){
        if (close(fd[i-1][1]) != 0) {
            
            perror("close");
            exit(1);
        }
        
        if ((result = read(fd[i-1][0],readbuf,sizeof(readbuf))) > 0){
            
            readbuf[0] = readbuf[0] + 1;
        }else{
            perror("read");
            exit(1);
        }
        if (close(fd[i][0]) != 0) {
            
            perror("close");
            exit(1);
        }
        if ((result = write(fd[i][1],readbuf,sizeof(readbuf))) < 0){
            perror("write");
            exit(1);
            printf("process #%d (%d) relaying message: %s",i,getpid(),readbuf);
        }
    }
    return 0;
}


    

