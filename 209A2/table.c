#include <stdio.h>
#include <string.h>
#include <stdlib.h>  /* for atoi() */
#include <unistd.h>  /* for getopt() */

static char *progname;
static int head = 0;


int main(int argc, char **argv)
{
    int status,c;
    head = 0;/*the default heading lines is 0.*/
    char line[1024];/*assign the line string.*/
    status = 0;
    progname = argv[0];
    extern int process(char *line, char *addr, int head);
    extern void process1(char *line, FILE *fp, int head);
    
    while ((c = getopt(argc, argv, "h:")) != EOF) {
        switch (c) {
            case 'h':
                head = atoi(optarg);
                
                break;
            case '?':
            default:
                status = 1;
                break;
        }
    }
    if (status){
        fprintf(stderr, "usage: table [-h] num [file ...]\n");
        return(status);
    }
    
    //if there is argument, program takes text line from standard input and never end.
    if (argc == 1){
        process1(line, stdin, head);
        return (status);
    }
     //there is -h argument but no input file.
    if (optind >= argc){
       process1(line, stdin, head);
        return (status);
    
    }else{
        for (; optind < argc; optind++){
        
       
            head = process(line, argv[optind], head);
        }
        
       
        
    }
    return(status);
    

}

//A help function to convert the text line to HTML code
int process(char *line, char *addr, int head){
    int i;
    FILE *fp;
    if((fp = fopen(addr, "r")) == NULL) {
        perror(addr);
        return 1;
    }
    while (fgets(line, 1024, fp) != NULL) /*read the line from fp*/
    {
        int l = strlen(line);// the length of the string line.
        if (head > 0){  //if there is head line
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
    return(head);
}

void process1(char *line, FILE *fp, int head){
    int i;
    while (fgets(line, 1024, fp) != NULL) /*read the line from fp*/
    {
        int l = strlen(line);// the length of the string line.
        if (head > 0){  //if there is head line
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
