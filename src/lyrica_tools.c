/* =============================== Libraries =============================== */
#include <stdbool.h>    
#include <stdlib.h>    
#include <stdint.h> 
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#ifdef WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

/* =========================== Funky Header Files ========================== */
#include "lyrica_tools.h"
#include "lyrica.h"

/* =============================== Functions =============================== */

void input_string(char string[])
{
    fgets(string, BUFF_STR, stdin);
    string[strlen(string) - 1] = '\0';
}
 
bool input_filechecker(char *filename)
{
    FILE *file_p = fopen(filename, "r");
    // Check whether the file exists at all
    if (file_p == false) return false;

    // Check whether the file has 0 bytes of data
    fseek(file_p, 0, SEEK_END);
    const size_t size = ftell(file_p);

    if (size > 0) {
        rewind(file_p);
        return true;
    } else {
        fclose(file_p);
        return false;
    }
}

bool list_files(const char *path)
{
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

void delay (const uint8_t minutes, const uint8_t seconds, const uint16_t miliseconds)
{
    static uint32_t time_previous = 0;

    /* In miliseconds */
    uint32_t time_raw = ((minutes * SECONDS * MILI) + (seconds * MILI) + miliseconds);
    uint32_t time_placement = time_raw - time_previous;

    #ifdef WIN32
        Sleep(time_placement * 100);
    #else
        usleep(time_placement * MILI);
        fflush(stdout);
    #endif

    time_previous = time_raw;
}
