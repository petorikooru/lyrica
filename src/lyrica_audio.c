/* =============================== Libraries =============================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================== Additional Libraries ========================= */
#include <gst/gst.h>
#include <taglib/tag_c.h>

/* =========================== Funky Header Files ========================== */
#include "lyrica.h"
#include "lyrica_audio.h"

char temp_string        [BUFF_STR];

/* =============================== Functions =============================== */

void* lyrica_audio_play(void *arg)
{
    char *song_path = (char*) arg;

    GstElement  *pipeline;
    GstBus      *bus;
    GstMessage  *msg;
    
    /* Initialize GStreamer */
    gst_init(NULL,NULL);

    /* Adjoin file:// uri to song path */
    snprintf(temp_string, sizeof(temp_string), 
            "%s%s %s", "filesrc location=" ,song_path, "! decodebin ! audioconvert ! alsasink");

    pipeline = gst_parse_launch (temp_string, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    if (msg != NULL) gst_message_unref(msg);

    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
}


Song lyrica_metadata(char *path, char *path_song)
{
    TagLib_File *file = taglib_file_new(path_song);

    const TagLib_Tag *tag = taglib_file_tag(file);
    const TagLib_AudioProperties *properties = taglib_file_audioproperties(file);

    Song song;
    if (tag != NULL) {
        strncpy(song.artist, taglib_tag_artist(tag), sizeof(song.artist));
        strncpy(song.title, taglib_tag_title(tag), sizeof(song.title));

        song.durations.total    = taglib_audioproperties_length(properties);
        song.durations.hours    = song.durations.total / 3600;
        song.durations.minutes  = (song.durations.total / 60) % 60;
        song.durations.seconds  = song.durations.total % 60;

        lyrica_metadata_picture(song.cover, path, file);
    }

    taglib_tag_free_strings();
    taglib_file_free(file);

    return song;
}


void lyrica_metadata_picture(char *image, char *path, TagLib_File *file)
{
    TagLib_Complex_Property_Attribute*** properties = taglib_complex_property_get(file, "PICTURE");

    TagLib_Complex_Property_Picture_Data picture;
    taglib_picture_from_complex_property(properties, &picture);

    snprintf(temp_string, BUFF_STR, "%s%s%s", path, "/", ".temp.png");
    FILE *image_file = fopen(temp_string, "wb");

    if (image_file != NULL) {
      fwrite(picture.data, picture.size, 1, image_file);
      fclose(image_file);
    }
    taglib_complex_property_free(properties);

    #ifdef WIN32
        snprintf(lyrics_path, BUFF_STR, "%s%s%s", path, "\\", path_lyrics);
    #else
        snprintf(image, BUFF_STR, "%s", temp_string);
    #endif
}
