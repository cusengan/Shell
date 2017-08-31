#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "functions.h"

#define bufferSize 100

int main(void){
    pid_t child;
    DIR* d;
    struct dirent* de;
    int i, c, k, j, num, stringSize = 10;
    int fileSize = 10, dirSize = 10;
    char s[256], cmd[256];
    int jump = 0;
    int checked = 0;
    char* buffer = (char*)calloc(bufferSize, sizeof(char));
    char** directories = (char**)calloc(dirSize, sizeof(char*));//array of strings (100)
    char** files = (char**)calloc(fileSize, sizeof(char*));//array of strings (100)
    printf("%s\n", directories[0]);

    printf("%s\n", directories[0]);
    time_t t;
    size_t bufsize = bufferSize;
    size_t hold;

    while(1){
        t = time( NULL );
        printf("Time: %s\n", ctime(&t));

        getcwd(s, sizeof(s));
        printf("\nCurrent Directory: %s \n", s);
        d = opendir(".");
        c = 0;
        while((de = readdir(d))){
            if(c == dirSize){
                dirSize *=2; //double array size
                directories = (char**)realloc(directories, dirSize);
            }
            if((de->d_type) & DT_DIR){
                directories[c] = (char*)calloc(strlen(de->d_name),sizeof(char));
                strcpy(directories[c], de->d_name);
                //printf("(%d Directory: %s) \n", c, directories[c]);
                c++;
            }

        }

        closedir(d);
        d = opendir(".");
        j = 0;
        while( (de = readdir(d) )){
            if(((de->d_type == DT_DIR))) continue;
            if(((de->d_type == DT_REG))){
                files[j] = (char*)calloc(strlen(de->d_name),sizeof(char));
                strcpy(files[j],de->d_name);
                //printf("(%d File: %s)\n", j, files[j]);
                j++;
            }

            if(j%8 == 0){
                printf("Hit N for Next\n");
                k=getchar();
            }
        }
        closedir(d);
        printf("------------------------------------------------------\n");
        printf("Menu:\n");
        printf("q: Quit\ne: Edit\nr: Run\nc: Change directories\n");
        num = getchar(); getchar();//need to clear near line
        switch(num){
            case 'q':free(buffer);
                     freeArray(directories, c);
                     freeArray(files, c);
                     free(directories);
                     free(files);
                     exit(0);
            case 'e':printf("Edit what?:");
                     scanf("%s", s);
                     strcpy(cmd, "pico ");
                     strcat(cmd, s);
                     system(cmd);
                     break;
            case 'r':printf("Run what?:\n");
                     hold = getLine(&buffer, &bufsize, stdin);
                     printf("%s",buffer);
                     system(buffer);
                     break;
            case 'c':printf("Change To?:");
                     freeArray(directories,c);
                     freeArray(files,c);
                     scanf("%s", cmd);
                     fseek(stdin,0,SEEK_END);//flush out input buffer
                     chdir(cmd);
                     break;
            default: printf("Command not understood\n");
        }

    }
}

