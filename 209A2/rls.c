#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/uio.h>

/* A recursive function to print the name of file/directory in the directory whose path is given by char *addr*/
int dir(char *addr,int argc)
{
    DIR *dp;
    struct dirent *p;
    char *name;
    char path[1024];//the string to store the target's path name
    char s[1024];
    extern int process(char *addr);
    int i;

    if ((dp = opendir(addr)) == NULL) {
	perror(addr);
	return(1);
    }
    while ((p = readdir(dp))){
        name = p->d_name;
        if ((strcmp(name,".") != 0) && (strcmp(name,"..") != 0)){
           strcpy(path,addr);
           strcat(path,"/");
           strcat(path,name);//assign the target's path name to the string
           if (process(path) == 0){
               if (argc == 1){    //for the special case which takes no argument.
                   for (i = 0; i + 2 < strlen(path); i++){
                       s[i] = path[i+2];//if get the string without "./" at beginning.
                   }
                   s[i] = '\0';
                   printf("%s\n",s);
                  
                   dir(path,argc);
               }else{
                 printf("%s\n",path);
                 dir(path,argc);//do the recursion to the subdirectory.
               }
           }else{
               if (argc == 1){  //for the special case which takes no argument.
                   for (i = 0; i + 2 < strlen(path); i++){
                       s[i] = path[i+2];
                   }
                   printf("%s\n",s);
                    s[i] = '\0';
               }else{
                 printf("%s\n",path);
               }
            }
        }
    }
    closedir(dp);
    return(0);
}

/* A help function to determine the given path is a directory or not,return 0 if it is a directory*/
int process(char *addr){
    int status = 1;
    struct stat statbuf;
    if (lstat(addr, &statbuf)) {
	    perror(addr);
    } else {
        if (S_ISDIR(statbuf.st_mode)){
            status = 0;
        }else if (S_ISREG(statbuf.st_mode)){
            status = 2;
        }else if (S_ISLNK(statbuf.st_mode)){
            status = 2;

        }
    }
    return(status);
}

/* the main block*/
int main(int argc, char **argv){
    extern int dir(char *addr,int argc);
    char *addr;
    int i;
    int result;//the return value of the help function
    
    for (i = 1; i < argc; i++){
         addr = argv[i];
        result = process (addr);
         if (result == 0){
             dir(addr,argc);
         }else if (result == 2){
             printf("%s: Not a directory\n",addr);
         }
    }
    
    /*handle the case which have no argument*/
    if (argc == 1){
        dir(".",argc);
    }
    return(0);
}
