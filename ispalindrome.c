#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *progname;

int main(int argc, char **argv)
{
    char *word;
    int p,q,l,status;
    status = 0;
    progname = argv[0];
    
    if (argc != 2){
        fprintf(stderr,"usage: %s [string]\n", progname);
        return 1;
    }
    word = argv[1];
    l = strlen(word);
    p = 0;
    q = l - 1;
    while (p < q){
        if (! isalnum(word[p])){
            p = p + 1;
        }else if(! isalnum(word[q])){
            q = q - 1;
        }else{
            if (tolower(word[p]) == tolower(word[q])){
                
                p = p + 1;
                q = q - 1;
            }else{
                status = 1;
                
                exit (1);
            }
        }
    }
    if (status == 0){
        
        exit (0);
        
    }
    return(status);
}
