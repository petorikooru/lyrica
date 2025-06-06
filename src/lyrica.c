/* Built-in Libraries */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/* Additional Libraries */
#include <gst/gst.h>        /* Play audio files */
#include <ncurses.h>        /* TUI stuff */
#include <pthread.h>        /* Play lyrics and audio in parallel */
#include <sixel.h>          /* Display song cover */
#include <taglib/tag_c.h>   /* Display audio metadata */

/* Funky Header files */
#include "lyrica.h"
#include "lyrica_tools.h"
#include "lyrica_audio.h"

char input_temp [BUFF_STR];

char path_dir   [BUFF_STR];
char path_song  [BUFF_STR];
char path_lyrics[BUFF_STR];

uint8_t term_col, term_row;

Song song;

int main()
{
    pthread_t thread_audio, thread_player, thread_time;

    bool status = lyrica_init();

    if (status == false) {
        fprintf(stderr, "FAILED to initialize player, exiting...");
        return 1;
    }

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
    // initscr();
    // getmaxyx(stdscr, term_row, term_col);
    // keypad(stdscr, TRUE);./
    // clear();

    system("clear");

    char mesg[] = "Please input the directory : ";
    printf("%s", mesg);
    // mvprintw(term_row / 2,(term_col - strlen(mesg)) / 2, "%s", mesg);

    // getstr(path_dir);
    input_string(path_dir);

    bool exist = list_files(path_dir);
    if (exist == false){
        clear();

        char err[] = "Directory Doesnt Exist! Exiting...";
        // mvprintw(term_row / 2,(term_col - strlen(err)) / 2, "%s", err);
        return false;
    }

    lyrica_song_input();
    lyrica_lyrics_input();
    
    song = lyrica_metadata(path_dir, path_song);

    return true;
}


void lyrica_song_input()
{
    fprintf(stdout, "Please select your music files: ");
    input_string(input_temp);
 
    #ifdef WIN32
        snprintf(song_path, BUFF_STR, "%s%s%s", path, "\\", path_song);
    #else
        snprintf(path_song, BUFF_STR, "%s%s%s", path_dir, "/", input_temp);
    #endif
}


void lyrica_lyrics_input()
{
    fprintf(stdout, "Please select your music files (.lrc): ");
    input_string(input_temp);
    
    #ifdef WIN32
        snprintf(lyrics_path, BUFF_STR, "%s%s%s", path, "\\", path_lyrics);
    #else
        snprintf(path_lyrics, BUFF_STR, "%s%s%s", path_dir, "/", input_temp);
    #endif
}


void lyrica_picture()
{
    sixel_encoder_t *image_output;

    sixel_encoder_new(&image_output, NULL);
    sixel_encoder_setopt(image_output, SIXEL_OPTFLAG_WIDTH, "200");

    sixel_encoder_encode(image_output, song.cover);
}


void *lyrica_time(void *arg){
    int s = 0;
    int m = 0;
    int h = 0;

    time_t counter = 0;
    time_t total   = song.durations.total;

     while (counter != total) {
        #include <unistd.h>
        sleep(1);
        fflush(stdout);
        printf(POS_YX YEL"[%02i:%02i:%02i] \n" RST, 6, 25, h, m, s);

        counter++;

        s++;
        if (counter % 60 == 0) {
            s = 0;
            m++;
        } else if (counter % 3600 == 0) {
            m = 0;
            h++;
        }
    }
}


void* lyrica_play(void *arg)
{
    time_t miliseconds;
    time_t seconds;
    time_t minutes;
    uint8_t i = 1;

    char line[BUFF_STR];
    char dump[BUFF_STR];
    char *lyrics;

    FILE* lyrics_file = fopen(path_lyrics, "r");
    bool check = input_filechecker(lyrics_file);
    if (check == false) return NULL;

    system("clear");

    lyrica_picture();
    printf(POS_YX MAG_BG BLK " Playing... \n" RST, 2, 25);
    printf(POS_YX CYN"%s - %s\n" RST, 3, 25, song.artist, song.title);
    printf(POS_YX BLU"Durations : %i minutes %i seconds \n\n" RST, 4, 25, song.durations.minutes, song.durations.seconds);

    while(fgets(line, sizeof(line), lyrics_file)){
        if (sscanf(line, "[%li:%li.%li]%s", &minutes, &seconds, &miliseconds, dump) == 4){

            delay(minutes, seconds, miliseconds);
            
            if (i == 1) {
                printf("\n");
                i++;
            }

            lyrics = strchr(line, ']');
            if (!lyrics || strcmp(lyrics, "\0") == 0) 
                printf(POS_YX LINE_ERASE, 8, 25);
            else if (lyrics != NULL)
                printf(POS_YX LINE_ERASE MAG "%s" RST, 8, 25, lyrics + 1);
        }
    }
}

void lyrica_end(int sig)
{
    remove(song.cover);
    exit(sig);
}
