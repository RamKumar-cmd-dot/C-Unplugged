#ifndef ALBUM_H
#define ALBUM_H

#include "songs.h"

typedef struct album_song {
    int song_id;
    struct album_song* next;
} album_song;

typedef struct album {
    int album_id;
    char album_name[80];
    album_song* song_head;
    struct album* next;
} album;

extern album* album_head_ptr;
extern int max_album_id;

void add_album(album* a);
void load_albums_from_file(const char* filename);
void save_albums_to_file(const char* filename);
void print_all_albums();
void show_album(int album_idx);
album* create_album(const char* name);
void delete_album(int idx);
void add_song_to_album(int album_idx);
void delete_song_from_album(int album_idx, int song_idx);

#endif
