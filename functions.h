#ifndef FUNCTIONS
#define FUNCTIONS
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <limits.h>
//#include <curses.h>


size_t getLine(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
char* readLine(){
    char* line = NULL;
    ssize_t bufsize = 0;
    getLine(&line, &bufsize, stdin);
    return line;
}

void allocateArray(char** array, int size, int stringSize){
    for(int i = 0; i < size; i++){
        array[i] = calloc(stringSize, sizeof(char));
    }
}

void freeArray(char** array, int size){
    //printf("%d\n",size);
    for(int i = 0; i < size; i++){
        free(array[i]);
    }

}


int getFileCreationTime(char *filePath){
    struct stat attrib;
    stat(filePath, &attrib);
    char date[10];
    strftime(date, 10, "%y%m%d", localtime(&(attrib.st_ctime)));
    //printf("The file %s was last modified at %s\n", filePath, date);
    int i = atoi(date);
    return i;
}

char* getCreationTimeString(char *filePath){
    struct stat attrib;
    stat(filePath, &attrib);
    char* date = (char*)calloc(10,sizeof(char));
    strftime(date, 10, "%m-%d-%y", localtime(&(attrib.st_ctime)));
    return date;
}



void fillDirArray(DIR* d, struct dirent* de, int* c, int* dirSize, char** array){
    d = opendir(".");
        *c = 0;
        while((de = readdir(d))){
            if(*c == 1024){
                printf("Directory size reached\n");
                break;
            }
            if((de->d_type) & DT_DIR){
                array[*c] = (char*)calloc(2048,sizeof(char));
                strcpy(array[*c], de->d_name);
                //printf("(%d Directory: %s) \n", c, array[c]);
                (*c)++;
            }

        }
        closedir(d);
}

void fillFileArray(DIR* d, struct dirent* de, int* j, int* fileSize, char** array, int* fileSizeArray, int* dates){
    FILE* fp;
    int size = 0;
    d = opendir(".");
        *j = 0;
        while( (de = readdir(d) )){
            if(*j == 1024){
                printf("File size reached");
                break;
            }
            if(((de->d_type == DT_DIR))) continue;
            if(((de->d_type == DT_REG))){
                fp = fopen(de->d_name, "r");
                fseek(fp, 0, 2);
                size = ftell(fp);
                fclose(fp);
                fileSizeArray[*j] = size;

                array[*j] = (char*)calloc(strlen(de->d_name)+1,sizeof(char));//need to have str length + 1 for the null character
                strcpy(array[*j],de->d_name);
                char* path = (char*)calloc(strlen(de->d_name)+3, sizeof(char));
                path[0] = '.';
                path[1] = '/';
                dates[*j] = getFileCreationTime(strcat(path,de->d_name));
                free(path);
                (*j)++;
                
            }
        }
        closedir(d);
}

void printFileArray(char** array, int* fileSizes, int* dates, int size, int jump){
    int show = 4*(jump+1);
    for(int i = 4*jump; i < show && i < size; i++){
        if(array[i] == NULL) break;
        char* path = (char*)calloc(strlen(array[i])+3, sizeof(char));
        path[0] = '.';
        path[1] = '/';
        strcat(path, array[i]);
        char* date = getCreationTimeString(path);
        printf("%d File: %s Size: %d bytes Date Last Modified: %s\n", i, array[i], fileSizes[i], date);
        free(path);
        free(date);
    }
}

void printDirArray(char** array, int size, int jump){
    int show = 4*(jump+1);
    //printf("%d\n", size);
    for(int i = 4*jump; i < show && i < size; i++){
        if(array[i] == NULL) break;
        printf("%d Directory: %s\n", i, array[i]);
    }
}

void swap(int* array, int first, int second){
    int temp = array[first];
    array[first] = array[second];
    array[second] = temp;
}

void swapStrings(char** s1, char** s2){
    char* temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}
int partition(int* mainArray, int* other, char** stringArray, int low, int high){
    int i = low -1;
    int pivot = mainArray[high];

    for(int j = low; j < high; j++){
        if(mainArray[j] <= pivot){
            i++;
            swap(mainArray,i, j);
            swap(other, i, j);
            
            swapStrings(&stringArray[i], &stringArray[j]);
            
           


        }
    }

    swap(mainArray, i+1, high);
    swap(other, i+1, high);
    swapStrings(&stringArray[i+1],&stringArray[high]);
    return i+1;


}
void quickSort(int* mainArray, int* other, char** stringArray, int low, int high){
    printf("\n");
    for(int i = 0; i <= high; i++){
        printf("%s-%d-%d\n", stringArray[i],mainArray[i],other[i]);
    }
    if(low < high){
        int pivot = partition(mainArray, other, stringArray, low, high);
        quickSort(mainArray, other, stringArray, low, pivot-1);
        quickSort(mainArray, other, stringArray, pivot+1, high);
    }

}







#endif // FUNCTIONS


