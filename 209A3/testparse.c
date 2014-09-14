#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 1024

int main(int argc, char **argv){
    extern int process(int num,char *readbuf);
    if (argc != 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    int num = atoi(argv[1]);
    if (num < 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    
    char readbuf[MAX_LEN];
    
    
    
    while (fgets(readbuf, MAX_LEN, stdin)) {
        
        
        
        process(num,readbuf);
        
           }
}


int process(int num,char *readbuf){
    int fd[num][2];
    int pid,status,result,i;
    for (i = 0; i < num - 1; i++){
        
        if (pipe(fd[i+1]) != 0){
            perror("pipe");
            exit(1);
        }
        
        if ((pid = fork()) == -1) {
            perror("fork");
            exit(1);
        }
        if (pid == 0){
            /* Child process closes up output side of pipe */
            
            if (close(fd[i][1]) != 0) {
                
                perror("close");
                exit(1);
            }
            
            if ((result = read(fd[i][0],readbuf,sizeof(readbuf))) > 0){
                
                readbuf[0] = readbuf[0] + 1;
            }else{
                perror("read");
                exit(1);
            }
            
            if (close(fd[i+1][0]) != 0) {
                
                perror("close");
                exit(1);
            }
            if ((result = write(fd[i+1][1],readbuf,sizeof(readbuf))) < 0){
                perror("write");
                exit(1);
            }
            
            printf("process #%d (%d) relaying message: %s",i,getpid(),readbuf);
        
        }else{
            /* Parent process closes up input side of pipe */
            /* Child process closes up output side of pipe */
            if (i == 0){
                if (close(fd[0][0]) != 0) {
                    
                    perror("close");
                    exit(1);
                }
                if ((result = write(fd[0][1],readbuf,sizeof(readbuf))) < 0){
                    perror("write");
                    exit(1);
                }
            }
            wait(&status);
            if (close(fd[num -1][1]) != 0) {
                
                perror("close");
                exit(1);
            }
            if ((result = read(fd[num - 1][0],readbuf,sizeof(readbuf))) > 0){
                
                printf("I hear %s",readbuf);
            }else{
                perror("read");
                exit(1);
            }
            return 0;
            
        }
    }
    return 0;
}