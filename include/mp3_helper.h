#ifndef MP3_HELPER_H
#define MP3_HELPER_H

#include "mongoose.h"

struct mp3_infos {
    struct mg_str title;    // Song title
    struct mg_str artist;   // Artist name
    unsigned int duration;  // Song duration in seconds
    struct mg_str album;    // Album name
    struct mg_str date;     // Release date
    struct mg_str filename; // File name
};

typedef struct mp3_infos mp3_infos;

mp3_infos* mp3_get_infos(char* artist, char* song_name);

#endif //MP3_HELPER_H