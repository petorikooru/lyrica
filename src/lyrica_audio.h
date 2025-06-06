#include <taglib/tag_c.h>   /* Display audio metadata */
#include "lyrica.h"

#ifndef BUFF_STR
#define BUFF_STR 512
#endif

#ifndef LYRICA_AUDIO
#define LYRICA_AUDIO

void* lyrica_audio_play(void *arg);

Song lyrica_metadata(char *path, char *path_song);
void lyrica_metadata_picture(char *image, char *path, TagLib_File *file);

#endif