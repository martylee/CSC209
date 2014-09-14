#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

int process(){
    int status,a,b;
    struct stat statbuf;
    DIR *dp;
    struct dirent *p;
    status = 0;
    if (lstat(".", &statbuf)) {
	    perror(".");
	    status = 1;
	}
    a = statbuf.st_ino;
    
    if (a == 2){
        printf("and then we're at the root directory.\n");
        return status;
        
    }else{
        chdir("..");
        if ((dp = opendir(".")) == NULL) {
            perror(".");
            status = 1;
            
        }
        while ((p = readdir(dp))){
            if (lstat(p->d_name, &statbuf)) {
                perror(p->d_name);
                status = 1;
            } else {
                b = statbuf.st_ino;
                if (b == a){
                    printf("%s\n", p->d_name);
                    break;
                }
            }
        }
        process();
    
    }
    return status;
}

int main(){
    extern int process();
    process();
    return(0);
}




