/* ============= Libraries ============ */
#include <stdint.h>     /* ::{int datatypes} */ 
#include <stdio.h>      /* ::{file datatype} */
#include <time.h>       /* ::{time datatype} */

/* ============== Macros ============= */
/*
    I'm too lazy
*/
#define BUFF_STR    512
#define RUNNING     1
#define SAME        0
#define None        false

/* =========== Checking... =========== */
#ifndef LYRICA_TOOLS
#define LYRICA_TOOLS

#define TODO(x) fprintf(stderr, "TODO: " x); exit(0);

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
bool input_filechecker(FILE *file);

/*
    List file in selected directory
    Return : noneint
*/
bool list_files(char *path);

/*
    Add delay
    Return : none
*/
void delay(time_t minutes, time_t seconds, time_t miliseconds);

#endif