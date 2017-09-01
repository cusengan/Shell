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
    int jumpDir = 0;//jump 4 dirs at a time
    int jumpFile = 0;//jump 4 files at a time
    int notChecked = 1;
    char* buffer = (char*)calloc(bufferSize, sizeof(char));
    char** directories = (char**)calloc(dirSize, sizeof(char*));//array of strings (10)
    char** files = (char**)calloc(fileSize, sizeof(char*));//array of strings (10)
    time_t t;
    size_t bufsize = bufferSize;
    size_t hold;

    while(1){
        t = time( NULL );
        printf("Time: %s\n", ctime(&t));

        getcwd(s, sizeof(s));
        printf("\nCurrent Directory: %s \n", s);
        if(notChecked){
            fillDirArray(d, de, &c, &dirSize, directories);
            fillFileArray(d, de, &j, &fileSize, files);
            notChecked = 0;
        }

        printf("Directories:\n");
        printArray(directories, c, jumpDir);
        printf("Files:\n");
        printArray(files,j, jumpFile);
        printf("------------------------------------------------------\n");
        printf("Menu:\n");
        printf("q: Quit\ne: Edit\nn: Next Directories\np: Previous Directories\nm: Next Files\nj: Previous Files\ns: Sort\nr: Run\nc: Change directories\n");
        num = getchar(); getchar();//need to clear near line
        switch(num){
            case 'n':if(4*(jumpFile+1) < c)
                        jumpFile++;
                     break;
            case 'p':if(jumpFile != 0)
                        jumpFile--;
                     break;
            case 'm':if(4*(jumpDir+1) < c)
                        jumpDir++;
                     break;
            case 'j':if(jumpDir != 0)
                        jumpDir--;
                     break;
            case 'q':free(buffer);
                     freeArray(directories, c);
                     freeArray(files, j);
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
                     notChecked = 1; //need to reset as to fill arrays again
                     jumpDir = 0;
                     jumpFile = 0;
                     chdir(cmd);
                     break;
            case 's':printf("d: Sort by date:\n");
                     printf("s: Sort by size:\n");
                     break;
            default: printf("Command not understood\n");
        }

    }
}

