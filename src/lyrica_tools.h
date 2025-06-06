/* =============================== Libraries =============================== */
#include <stdint.h> 
#include <stdio.h>
#include <time.h> 

/* ================================= Macros ================================ */
#define BUFF_STR    512
#define RUNNING     1
#define SAME        0
#define None        false

#define SECONDS     60
#define NANO        1e9
#define MILI        1000

#ifndef LYRICA_TOOLS
#define LYRICA_TOOLS

#define TODO(x) fprintf(stderr, "TODO: " x); exit(0);

/* =============================== Functions =============================== */

/*
    Handle string input
    Return : none
 */
void input_string(char string[]);

/*
    Handle number input
    Return : 32 bit integer
*/
int32_t input_number();

/*
    Handle file input
    Return : bool
*/
bool input_filechecker(char *filename);

/*
    List file in selected directory
    Return : noneint
*/
bool list_files(const char *path);

/*
    Add delay
    Return : none
*/
void delay(const uint8_t minutes, const uint8_t seconds, const uint16_t miliseconds);

#endif