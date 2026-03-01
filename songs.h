#ifndef SONGS_H
#define SONGS_H

typedef struct song {
    int song_id;
    char song_name[80];
    char artist_name[80];
    char duration[16];
    struct song* next;
} song;

extern song* song_library_head;
extern int max_song_id;

int load_song_library(const char* filename);
void list_all_songs();
void add_new_song_to_library();
song* find_song_by_id(int id);
void trim_us(char* s);

#endif
