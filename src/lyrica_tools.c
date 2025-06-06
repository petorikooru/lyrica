/* ============= Libraries ============ */
#include <stdbool.h>     // ::{bool}
#include <stdlib.h>      // ::{malloc, free, size_t} 
#include <stdint.h>      // ::{int}
#include <stdio.h>       // ::{fgets, getchar, fseek, rewind, fclose}
#include <string.h>      // ::{strlen, strcmp}
#include <dirent.h>

#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#include "lyrica_tools.h"

/* ============ Functions ============ */

void input_string(char string[]){
    fgets(string, BUFF_STR, stdin);
    string[strlen(string) - 1] = '\0';
}

int32_t input_number(){
    int32_t integer;
    char input[BUFF_STR];
    input_string(input);

    // Check whether it is truly an integer
    sscanf(input, "%d", &integer);

    return integer;
}
 
bool input_filechecker(FILE *file){
    // Check whether the file exists at all
    if (file == false) {
        return false;
    }

    // Check whether the file has 0 bytes of data
    fseek(file, 0, SEEK_END);
    const size_t size = ftell(file);

    if (size > 0){
        rewind(file);
        return true;
    } else {
        fprintf(stderr, "FILE is empty!");

        fclose(file);
        return false;
    }
}

bool list_files(char *path){
    DIR *dir_p;
    struct dirent *dir;
    dir_p = opendir(path);

    if (dir_p != NULL) {
        while ((dir = readdir(dir_p)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(dir_p);
        return true;
    } else {
        fprintf(stderr, "Directory not found!\n");
        return false;
    }
}

void delay (time_t minutes, time_t seconds, time_t miliseconds){
    static uint32_t time_previous = 0;

    fflush(stdout);

    /* In miliseconds */
    uint32_t time_raw = ((minutes * 60 * 1000) + (seconds * 1000) + miliseconds);
    uint32_t time_placement = time_raw - time_previous;

    #ifdef WIN32
        Sleep(time_placement * 100);
    #else
        usleep(time_placement * 1000);
    #endif

    time_previous = time_raw;
}
