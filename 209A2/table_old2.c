#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* for atoi() */
#include <unistd.h>  /* for getopt() */

int main(int argc, char **argv)
{
    FILE *fp;
    int f, head,status,c,flag;
    head = 0;/*the default heading lines is 0.*/
    char line[1024];/*assign the line string.*/
    f = 1;
    status = 0;
    flag = 0;/*determine whether the input is files or stdin*/
    extern void process(char *line, FILE *fp, int head);
    if (argc == 1){
        flag = 1;
    }
    
    while ((c = getopt(argc, argv, "h:")) != EOF) {
        switch (c) {
            case 'h':
                head = atoi(optarg);
                f = optind;
                /*printf("%d,%d\n",head,f);*/
                if (f > argc - 1){
                    flag = 1;
                }
                break;
            case '?':
                fprintf(stderr, "usage: table [-h] num [file ...]\n");
                return (1);
            default:
                status = 1;
                break;
        }
    }
    
    if (flag == 1){
        process(line, stdin, head);
        
        return (status);
    }

    
    while ( f < argc){
        if ((fp = fopen(argv[f], "r")) == NULL) {
            perror(argv[f]);
            status = 1;
        }else{
            process(line, fp, head);
            if (head >= 0){
                printf("</th></tr>\n");
            }else{
                printf("</td></tr>\n");
            }
        }
        
        fclose(fp);
        f = f + 1;
    }
    return(status);
}

void process(char *line, FILE *fp, int head){
    int i;
    while (fgets(line, 1024, fp) != NULL) /*read the line from fp*/
    {
        int l = strlen(line);
        if (head > 0){
            printf("<tr><th>");
            for (i = 0; i < l; i++){
                if (line[i] == '\n'){
                    printf("</th></tr>\n");
                    
                }else if (line[i] == '\t'){
                    printf("</th><th>");
                    
                }else{
                    printf("%c",line[i]);
                }
            }
            head = head - 1;
        }
        else{
            printf("<tr><td>");
            for (i = 0; i < l; i++){
                if (line[i] == '\n'){
                    printf("</td></tr>\n");
                    
                }else if (line[i] == '\t'){
                    printf("</td><td>");
                    
                }else{
                    printf("%c",line[i]);
                }
            }
        }
    }
}
