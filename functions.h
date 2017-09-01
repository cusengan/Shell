#ifndef FUNCTIONS.H
#define FUNCTIONS.H
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

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

        closedir(d);
}

void fillFileArray(DIR* d, struct dirent* de, int* j, int* fileSize, char** array){
    d = opendir(".");
        *j = 0;
        while( (de = readdir(d) )){
            if(((de->d_type == DT_DIR))) continue;
            if(*j == *fileSize){
                *fileSize *=2; //double array size
                array = (char**)realloc(array, *fileSize);
            }
            if(((de->d_type == DT_REG))){
                array[*j] = (char*)calloc(strlen(de->d_name),sizeof(char));
                strcpy(array[*j],de->d_name);
                //printf("(%d File: %s)\n", j, array[j]);
                (*j)++;
            }
        }

        closedir(d);
}

void printArray(char** array, int size, int jump){
    int show = 4*(jump+1);
    for(int i = 4*jump; i < show && i < size; i++){
        printf("(%d File: %s)\n", i, array[i]);
    }
}




#endif // FUNCTIONS


