#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "parse.h"

#define MAXLEN 1024

int laststatus = 0;

int main()
{
    char buf[1000], **p;
    extern void execute(char **argv);
    
    while (printf("$ "), fgets(buf, sizeof buf, stdin))
        if ((p = parse(buf)))
            execute(p);
    
    
    
    return(laststatus);
}

void execute(char **argv){
    
    extern char **environ;
    int exist = 0;
    int flag = 0;
    const char *slash = "/";
        
    char path1[20],path2[20],path3[20],path4[20];
    char realpath[1024];
    struct stat statbuf;
    strcpy(path1,"/bin/");
    strcpy(path2,"/usr/bin/");
    strcpy(path3,"/usr/local/bin/");
    strcpy(path4,"./");
    
    char *c = argv[0];
    while (*c){
        if (strchr(slash, *c)){
            flag = 1;
            break;
        }
        c++;
    }
    
    int x = fork();
    if (x == 0) {
        if (flag == 0){
            strcpy(realpath,path1);
            strcat(realpath,argv[0]);
            if (stat(realpath, &statbuf) != 0) {
                strcpy(realpath,path2);
                strcat(realpath,argv[0]);
                if (stat(realpath, &statbuf) != 0) {
                    strcpy(realpath,path3);
                    strcat(realpath,argv[0]);
                    if (stat(realpath, &statbuf) != 0) {
                        strcpy(realpath,path4);
                        strcat(realpath,argv[0]);
                        if (stat(realpath, &statbuf) != 0) {
                            exist = 1;
                        }
                    }
                }
            }
           
            if (exist != 0){
                printf("%s: Command not found\n",argv[0]);
            }else{
                execve(realpath,argv,environ);
                perror(realpath);
            }
        }else{
            execve(argv[0],argv,environ);
            perror(argv[0]);
        }
    }else {
        int pid, laststatus;
        pid = wait(&laststatus);
    }
}

