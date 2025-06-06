/* =============================== Libraries =============================== */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/* ========================== Additional Libraries ========================= */
#include <gst/gst.h>        /* Play audio files                     */
#include <ncurses.h>        /* TUI stuff                            */
#include <pthread.h>        /* Play lyrics and audio in parallel    */
#include <sixel.h>          /* Display song cover                   */
#include <taglib/tag_c.h>   /* Display audio metadata               */

/* Sleep */
#ifdef WIN32
    #include <Windows.h>
#else    
    #include <unistd.h>        
#endif 

/* =========================== Funky Header Files ========================== */

#include "lyrica.h"
#include "lyrica_tools.h"
#include "lyrica_audio.h"

/* ============================ Global Variables =========================== */

char input_temp [BUFF_STR];

char path_dir   [BUFF_STR];
char path_song  [BUFF_STR];
char path_lyrics[BUFF_STR];

uint8_t term_col, term_row;

Song song;

/* =============================== Functions =============================== */

int main()
{
    pthread_t thread_audio, thread_player, thread_time;

    bool status = lyrica_init();
    if (status == false) {
        fprintf(stderr, "FAILED to initialize player, exiting...");
        return 1;
    }

    printf(CURSOR_HIDE);
    signal(SIGINT, lyrica_end);

    pthread_create(&thread_audio, NULL, lyrica_audio_play, &path_song);
    pthread_create(&thread_player, NULL, lyrica_play, NULL);
    pthread_create(&thread_time, NULL, lyrica_time, NULL);
    pthread_join(thread_audio, NULL);
    pthread_join(thread_player, NULL);
    pthread_join(thread_time, NULL);

    lyrica_end(SIGTERM);
}


bool lyrica_init()
{
    system("clear");

    printf("Please input the directory : ");
    input_string(path_dir);

    bool exist_dir = list_files(path_dir);
    if (exist_dir == false) {
        fprintf(stderr, "Directory Doesnt Exist! Exiting...");
        return false;
    }

    bool exist_file = lyrica_input();
    if (exist_file == false) {
        fprintf(stderr, "File Doesnt Exist! Exiting...");
        return false;
    }

    song = lyrica_metadata(path_dir, path_song);

    return true;
}


bool lyrica_input(){
    fprintf(stdout, "Please select your music files (without file extension): ");
    input_string(input_temp);
 
    #ifdef WIN32
        snprintf(path_song, BUFF_STR, "%s%s%s%s", path_dir, "\\", input_temp, ".flac");
    #else
        snprintf(path_song, BUFF_STR, "%s%s%s%s", path_dir, "/", input_temp, ".flac");
    #endif

    bool exist_song = input_filechecker(path_song);
    if (exist_song == false) return false;

    /* Check whether it has lrc file*/
    #ifdef WIN32
        snprintf(path_lyrics, BUFF_STR, "%s%s%s%s", path_dir, "\\", input_temp, ".lrc");
    #else
        snprintf(path_lyrics, BUFF_STR, "%s%s%s%s", path_dir, "/", input_temp, ".lrc");
    #endif

    bool exist_lyrics = input_filechecker(path_lyrics);
    if (exist_lyrics == false) return false;

    return true;
}


void lyrica_picture()
{
    sixel_encoder_t *image_output;

    sixel_encoder_new(&image_output, NULL);
    sixel_encoder_setopt(image_output, SIXEL_OPTFLAG_WIDTH, "200");

    sixel_encoder_encode(image_output, song.cover);
}


void *lyrica_time(void *arg)
{
    uint8_t s = 0;
    uint8_t m = 0;
    uint8_t h = 0;

    uint16_t counter = 0;
    uint16_t total   = song.durations.total;

     while (counter != total) {
        counter++;
        if (counter % 60 == 0 && counter != 0) {
            s = 0;
            m++;
        } else if (counter % 3600 == 0 && counter != 0) {
            m = 0;
            h++;
        }

        printf(POS_YX YEL"[%02i:%02i:%02i]" RST, 6, 25, h, m, s);

        #ifdef WIN32
            Sleep(1);
        #else
            sleep(1);
        #endif

        fflush(stdout);

        s++;
    }
}


void* lyrica_play(void *arg)
{
    uint16_t miliseconds;
    uint8_t seconds;
    uint8_t minutes;

    char line[BUFF_STR];
    char dump[BUFF_STR];
    char *lyrics;

    FILE* lyrics_file = fopen(path_lyrics, "r");

    system("clear");

    lyrica_picture();
    printf(POS_YX MAG_BG BLK " Playing..." RST, 2, 25);
    printf(POS_YX CYN"%s - %s" RST, 3, 25, song.artist, song.title);
    printf(POS_YX BLU"Durations : %i minutes %i seconds" RST, 4, 25, song.durations.minutes, song.durations.seconds);

    while(fgets(line, sizeof(line), lyrics_file)) {
        if (sscanf(line, "[%2hhi:%2hhi.%hi]%s", &minutes, &seconds, &miliseconds, dump) == 4){
            /* Somehow it takes some random ass number on the back */
            if (miliseconds >= 100) miliseconds = miliseconds / 10;

            delay(minutes, seconds, miliseconds);

            lyrics = strchr(line, ']');
            printf(POS_YX LINE_ERASE MAG "%s" RST, 8, 25, lyrics + 1);

        } else if (sscanf(line, "[%2hhi:%2hhi.%hi]", &minutes, &seconds, &miliseconds) == 3){
            /* Somehow it takes some random ass number on the back */
            if (miliseconds >= 100) miliseconds = miliseconds / 10;

            delay(minutes, seconds, miliseconds);

            printf(POS_YX LINE_ERASE, 8, 25);
        }
    }
}

void lyrica_end(int sig)
{
    printf(POS_YX RST, 13, 1);
    remove(song.cover);
    exit(sig);
}
