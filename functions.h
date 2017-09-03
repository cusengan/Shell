#ifndef FUNCTIONS.H
#define FUNCTIONS.H
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <conio.h>


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



void fillDirArray(DIR* d, struct dirent* de, int* c, int* dirSize, char** array){
    d = opendir(".");
        *c = 0;
        while((de = readdir(d))){
            if(*c == *dirSize){
                *dirSize *=2; //double array size
                array = (char**)realloc(array, *dirSize);
            }
            if((de->d_type) & DT_DIR){
                array[*c] = (char*)calloc(strlen(de->d_name),sizeof(char));
                strcpy(array[*c], de->d_name);
                //printf("(%d Directory: %s) \n", c, array[c]);
                (*c)++;
            }

        }
        (*c)++;
        closedir(d);
}

void fillFileArray(DIR* d, struct dirent* de, int* j, int* fileSize, char** array, int* fileSizeArray){
    FILE* fp;
    int size = 0;
    d = opendir(".");
        *j = 0;
        while( (de = readdir(d) )){
            if(((de->d_type == DT_DIR))) continue;
            if(*j == *fileSize){
                *fileSize *=2; //double array size
                array = (char**)realloc(array, *fileSize);
                fileSizeArray = (int*)realloc(fileSizeArray, *fileSize);
            }
            if(((de->d_type == DT_REG))){
                array[*j] = (char*)calloc(strlen(de->d_name),sizeof(char));
                strcpy(array[*j],de->d_name);
                fp = fopen(de->d_name, "r");
                fseek(fp, 0, 2);
                size = ftell(fp);
                fileSizeArray[*j] = size;
                (*j)++;
                fclose(fp);
            }
        }
        (*j)++;
        closedir(d);
}

void printFileArray(char** array, int size, int jump){
    int show = 4*(jump+1);
    for(int i = 4*jump; i < show && i < size; i++){
        if(array[i] == NULL) break;
        printf("(%d File: %s)\n", i, array[i]);
    }
}

void printDirArray(char** array, int size, int jump){
    int show = 4*(jump+1);
    for(int i = 4*jump; i < show && i < size; i++){
        if(array[i] == NULL) break;
        printf("(%d Directory: %s)\n", i, array[i]);
    }
}

void swap(int* array, int first, int second){
    int temp = array[first];
    array[first] = array[second];
    array[second] = temp;
}
int partition(int* array, char** stringArray, int low, int high){
    int i = low -1;
    int pivot = array[high];

    for(int j = low; j < high; j++){
        if(array[j] <= pivot){
            i++;
            swap(array,i, j);
            char* temp = (char*)calloc(strlen(stringArray[j]), sizeof(char));
            strcpy(temp,stringArray[j]);
            stringArray[j] = realloc(stringArray[j], strlen(stringArray[i]));
            strcpy(stringArray[j], stringArray[i]);
            stringArray[i] = realloc(stringArray[i], strlen(temp));
            strcpy(stringArray[i], temp);
            free(temp);


        }
    }

    swap(array, i+1, high);
    return i+1;


}
void quickSort(int* array, char** stringArray, int low, int high){
    if(low < high){
        int pivot = partition(array, stringArray, low, high);
        quickSort(array, stringArray, low, pivot -1);
        quickSort(array, stringArray, pivot + 1, high);
    }
}








#endif // FUNCTIONS


