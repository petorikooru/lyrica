/* =============================== Libraries =============================== */
#include <taglib/tag_c.h>
#include "lyrica.h"

/* ================================= Macros ================================ */
#ifndef BUFF_STR
#define BUFF_STR 512
#endif

#ifndef LYRICA_AUDIO
#define LYRICA_AUDIO

/* =============================== Functions =============================== */
void* lyrica_audio_play(void *arg);
Song lyrica_metadata(char *path, char *path_song);
void lyrica_metadata_picture(char *image, char *path, TagLib_File *file);

#endif