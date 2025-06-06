#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <taglib/tag_c.h>

#ifndef BUFF_STR
#define BUFF_STR 512
#endif

/* Terminal Color Controls */
#define BLK             "\e[30m"
#define BLK_BG          "\e[40m"
#define RED             "\e[31m"
#define RED_BG          "\e[41m"
#define GRN             "\e[32m"
#define GRN_BG          "\e[42m"
#define YEL             "\e[33m"
#define YEL_BG          "\e[43m"
#define BLU             "\e[34m"
#define BLU_BG          "\e[44m"   
#define MAG             "\e[35m"
#define MAG_BG          "\e[45m"
#define CYN             "\e[36m"
#define CYN_BG          "\e[46m"
#define WHT             "\e[37m"
#define WHT_BG          "\e[47m"
#define GRY             "\e[90m"
#define GRY_BG          "\e[100m"
#define PNK             "\e[91m"
#define PNK_BG          "\e[101m"
#define LME             "\e[92m"
#define LME_BG          "\e[102m"
#define RST             "\e[0m"

/* Cursor and Terminal controls */
#define LINE_UP     "\e[A"
#define LINE_ERASE  "\e[0K"
#define POS_YX       "\e[%d;%dH"


#ifndef LYRICA
#define LYRICA

typedef struct {
    uint16_t    total;
    uint8_t     hours;
    uint8_t     minutes;
    uint8_t     seconds;
} Durations;

typedef struct {
    char        title[BUFF_STR];
    char        artist[BUFF_STR];
    char        cover[BUFF_STR];

    Durations   durations;
} Song;

bool lyrica_init();
void lyrica_end(int sig);

void lyrica_song_input();
void lyrica_lyrics_input();
void lyrica_picture();
void *lyrica_time(void *arg);
void *lyrica_play(void *arg);

#endif