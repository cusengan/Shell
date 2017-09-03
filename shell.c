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
    int i, c, k, j, num, stringSize = 10;
    int fileSize = 10, dirSize = 10;
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

    while(1){
        t = time( NULL );
        printf("Time: %s\n", ctime(&t));

        getcwd(s, sizeof(s));
        printf("\nCurrent Directory: %s \n", s);
        if(notChecked){
            fillDirArray(d, de, &c, &dirSize, directories);
            fillFileArray(d, de, &j, &fileSize, files, fileSizeArray);
            notChecked = 0;
        }
        printf("Directories:\n");
        printDirArray(directories, c, jumpDir);
        printf("Files:\n");
        printFileArray(files,j, jumpFile);
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
            case 'm':if(4*(jumpFile+1) < c)
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
            case 'r':printf("Run what?:\n");
                     hold = getLine(&buffer, &bufsize, stdin);
                     printf("%s",buffer);
                     system(buffer);
                     break;
            case 'c':printf("Change To?:");
                     freeArray(directories,c);
                     freeArray(files,j);
                     scanf("%s", cmd);
                     while((getchar() != '\n'));// clears input buffer//flush out input buffer
                     notChecked = 1; //need to reset as to fill arrays again
                     jumpDir = 0;
                     jumpFile = 0;
                     chdir(cmd);
                     break;
            case 's':printf("d: Sort by date:\n");
                     printf("s: Sort by size:\n");
                     quickSort(fileSizeArray, files, 0, j-1);
                     break;
            default: printf("Command not understood\n");
        }

    }
}

