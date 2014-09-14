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
    int id,i,flag;
    flag = 1;
    if (argc != 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    int num = atoi(argv[1]);
    if (num < 2){
        fprintf(stderr, "usage: ring ringsize\nring size must be at least 2\n");
        return 1;
    }
    int fd[num][2];
    char readbuf[MAX_LEN];
    int pid,status,result;
    
    
    while (fgets(readbuf, MAX_LEN, stdin)) {
        
        for (i = 0; i < num; i++){
            
            if (pipe(fd[i]) != 0){
                perror("pipe");
                exit(1);
            }
        }
        for (i = 0; i < num; i++){

        
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
                    if (i == num - 1){
                        printf("I hear %s",readbuf);
                        break;
                        
                    }
                    
                    readbuf[0] = readbuf[0] + 1;
                
                }else{
                    perror("read");
                    exit(1);
                }
                
                
                
            }else{
                /* Parent process closes up input side of pipe */
                /* Child process closes up output side of pipe */
                
                if (strcmp (readbuf,"") == 0){
                    if (i == 1){
                        printf("process #0 (%d) exiting due to EOF on stdin",getpid());
                    }else{
                        printf("process #%d (%d) exiting due to EOF on frompipe",i,getpid());
                    }
                    if ((close(fd[i+1][0]) && (close(fd[i+1][1])))!= 0) {
                        
                        perror("close");
                        exit(1);
                    }
                    
                }
                
                if (close(fd[i][0]) != 0) {
                    
                    perror("close");
                    exit(1);
                }
                if ((result = write(fd[i][1],readbuf,sizeof(readbuf))) < 0){
                    perror("write");
                    exit(1);
                }
                
                
                
                if (i == 0){
                    id = getpid();
                    printf("process #0 (%d) sending message: %s",id,readbuf);
                    
                }else if (i == num - 1){
                    
                    printf("process #%d (%d) relaying message: %s",i,getpid(),readbuf);
                }else{
                    printf("process #%d (%d) relaying message: %s",i,getpid(),readbuf);
                }
                wait(&status);
                
            }
        }
        
    }
}
