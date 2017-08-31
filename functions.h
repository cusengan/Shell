#ifndef FUNCTIONS.H
#define FUNCTIONS.H


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
    //printf("%d",size);
    for(int i = 0; i < size; i++){
        free(array[i]);
    }
}

void fillArrays(DIR* d, struct dirent* de);




#endif // FUNCTIONS


