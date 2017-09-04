#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include <curses.h>
#include <sys/wait.h>
#define bufferSize 100

int main(void){
    pid_t child;
    DIR* d;
    struct dirent* de;
    int i=0, c=0, k=0, j=0, num=0, stringSize = 10;
    int fileSize = 1024, dirSize = 1024;
    char s[256], cmd[256];
    int jumpDir = 0;//jump 4 dirs at a time
    int jumpFile = 0;//jump 4 files at a time
    int notChecked = 1;
    time_t t;
    size_t bufsize = bufferSize;
    size_t hold;

    char* buffer = (char*)calloc(bufferSize, sizeof(char));
    char** directories = (char**)calloc(dirSize, sizeof(char*));//array of strings (10)
    char** files = (char**)calloc(fileSize, sizeof(char*));//array of strings (10)
    int* fileSizeArray = (int*)calloc(fileSize, sizeof(int));
    int* dates = (int*)calloc(fileSize, sizeof(int));

    while(1){
        t = time( NULL );
        printf("Time: %s\n", ctime(&t));

        getcwd(s, sizeof(s));
        printf("\nCurrent Directory: %s \n", s);
        if(notChecked){
            fillDirArray(d, de, &c, &dirSize, directories);
            fillFileArray(d, de, &j, &fileSize, files, fileSizeArray, dates);
            notChecked = 0;
        }
        printf("Directories:\n");
        printDirArray(directories, c, jumpDir);
        printf("Files:\n");
        printFileArray(files, fileSizeArray, dates, j, jumpFile);
        printf("------------------------------------------------------\n");
        printf("Menu:\n");
        printf("q: Quit\ne: Edit\nn: Next Directories\np: Previous Directories\nm: Next Files\nj: Previous Files\ns: Sort\nr: Run\nc: Change directories\n");
        num = getchar(); getchar();//need to clear new line
        switch(num){
            case 'n':if(4*(jumpDir+1) < c)
                        jumpDir++;
                     break;
            case 'p':if(jumpDir != 0)
                        jumpDir--;
                     break;
            case 'm':if(4*(jumpFile+1) < j)
                        jumpFile++;
                     break;
            case 'j':if(jumpFile != 0)
                        jumpFile--;
                     break;
            case 'q':free(buffer);
                     freeArray(directories, c);
                     freeArray(files, j);
                     free(directories);
                     free(files);
                     free(fileSizeArray);
                     free(dates);
                     exit(0);
            case 'e':child = fork();
                     if(child == 0){
                     //printf("in child");
                     printf("Edit what with vim? (Choose a file number):");
                     int choice;
                     scanf("%d", &choice);
                     if(choice >= j || choice < 0){
                        printf("invalid choice\n");
                        exit(127);
                     }
                     char* cdo = "vim";
                     char* argv[3];
                     argv[0] = "vim";
                     argv[1] = files[choice];
                     argv[2] = NULL;
                     execvp(cdo,argv);
                     exit(127);
                     }else{
                         wait(0);
                         break;
                     }                    
            case 'r':printf("Run what? (Choose a file number):\n");
                     int program;
                     scanf("%d", &program);
                     if(program >= j || program < 0){
                        printf("invalid choice\n");
                        while((getchar() != '\n'));
                        break;
                     }
                     char* path = (char*)calloc(strlen(files[program])+3, sizeof(char));
                     path[0] = '.';
                     path[1] = '/';
                     strcat(path, files[program]);
                     system(path);
                     while((getchar() != '\n'));
                     free(path);
                     break;
            case 'c':printf("Change To? (Choose a directory number):");
                     int choice;
                     scanf("%d", &choice);
                     if(choice >= c || choice < 0){
                        printf("invalid choice\n");
                        while((getchar() != '\n'));
                        break;
                     }
                     chdir(directories[choice]);
                     freeArray(directories,c);
                     freeArray(files,j); 
                     while((getchar() != '\n'));// clears input buffer//flush out input buffer
                     notChecked = 1; //need to reset as to fill arrays again
                     jumpDir = 0;
                     jumpFile = 0;
                     break;
            case 's':printf("0: Sort by date:\n");
                     printf("1: Sort by size:\n");
                     scanf("%d", &choice);
                     if(choice == 0)
                        quickSort(dates, fileSizeArray, files, 0, j-1);
                     else if(choice == 1)
                        quickSort(fileSizeArray, dates, files, 0, j-1);
                     else printf("invalid choice\n");
                     while((getchar() != '\n'));
                     break;
            default: printf("Command not understood\n");
        }

    }
}

