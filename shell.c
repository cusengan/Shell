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
#define BUFFERSIZE 100
#define ARGUMENTSIZE 100

int main(void){
    pid_t child;
    DIR* d;
    struct dirent* de;
    int i=0, c=0, k=0, j=0, num=0, stringSize = 10;
    int fileSize = 1024, dirSize = 1024;
    char s[500];
    int jumpDir = 0;//jump 4 dirs at a time
    int jumpFile = 0;//jump 4 files at a time
    int notChecked = 1;
    time_t t;
    size_t bufsize = BUFFERSIZE;
    size_t hold;

    char* buffer = (char*)calloc(BUFFERSIZE, sizeof(char));
    char** directories = (char**)calloc(dirSize, sizeof(char*));//array of strings (10)
    char** files = (char**)calloc(fileSize, sizeof(char*));//array of strings (10)
    int* fileSizeArray = (int*)calloc(fileSize, sizeof(int));
    int* dates = (int*)calloc(fileSize, sizeof(int));

    while(1){
        t = time( NULL );
        printf("Time: %s\n", ctime(&t));

        if(getcwd(s, sizeof(s)) == NULL)
            printf("Directory path is too long to be shown\n");
        else
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
        num = getchar(); //need to clear new line
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
                     printf("Edit what with vim? (Choose a file number):");
                     int choice;
                     int return_choice = 0;
                     while(!return_choice){
                        return_choice = scanf("%d", &choice);
                        if(!return_choice){
                            printf("invalid input\n");
                            break;
                        }
                     }

                     if(!return_choice){
                        while((getchar() != '\n'));
                        exit(127);
                     }
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
                     int file_input = 0;
                     while(!file_input){
                        file_input = scanf("%d", &program);
                        if(!file_input){
                            printf("invalid input\n");
                            break;
                        }
                     }
                    if(!file_input){
                        while((getchar() != '\n'));
                        break;
                    }
                     if(program >= j || program < 0){
                        printf("invalid choice\n");
                        while((getchar() != '\n'));
                        break;
                     }
                     //done error checking
                     printf("How many arguments do you want to pass to the command line?: \n");
                     int args = 0;
                     file_input = 0;
                     while(!file_input){
                        file_input = scanf("%d", &args);
                        if(!file_input){
                            printf("invalid input\n");
                            break;
                        }
                     }
                    if(!file_input){
                        while((getchar() != '\n'));
                        break;
                    }

                    while((getchar() != '\n'));

                    printf("%d\n",args);
                    printf("Type in your arguments one at a time (argument size is 100):\n");
                    char** arguments = (char**)calloc(args, sizeof(char*));
                    int argumentSize = 0;
                    for(int i = 0; i < args; i++){
                        arguments[i] = readLine();
                        argumentSize+=strlen(arguments[i]);
                    }

                    printf("%d\n",argumentSize );
                    

                     char* path = (char*)calloc(strlen(files[program])+3 + 1 + argumentSize, sizeof(char));
                     path[0] = '.';
                     path[1] = '/';
                     strcat(path, files[program]);
                     strcat(path, " ");
                     for(int i = 0; i < args; i++){
                        strcat(path, arguments[i]);
                     }
                     system(path);
                     freeArray(arguments, args);
                     free(arguments);
                     free(path);
                     break;
            case 'c':printf("Change To? (Choose a directory number):");
                     int choice;
                     int return_choice = 0;
                     while(!return_choice){
                        return_choice = scanf("%d", &choice);
                        if(!return_choice){
                            printf("invalid input\n");
                            break;
                        }
                     }
                    if(!return_choice){
                        while((getchar() != '\n'));
                        break;
                    }

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
                     int return_value = 0;
                     while(!return_value){
                        return_value = scanf("%d", &choice);
                        if(!return_value) printf("invalid input\n");
                        break;
                     }
                     if(!return_value){
                        while((getchar() != '\n'));
                        break;
                     }
                     
                     if(choice == 0)
                        quickSort(dates, fileSizeArray, files, 0, j-1);
                     else if(choice == 1)
                        quickSort(fileSizeArray, dates, files, 0, j-1);
                     else printf("invalid choice\n");
                     break;
            default: printf("Command not understood\n");
        }

    }
}

