#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "songs.h"

typedef struct playlist_node {
    song* song_ptr;
    struct playlist_node* next;
    struct playlist_node* prev;
} playlist_node;

extern playlist_node* playlist_head;
extern playlist_node* playlist_tail;
extern playlist_node* current_play;

void add_song_to_playlist();
void add_album_to_playlist(int album_idx,playlist_node** ph,playlist_node** pt);
void view_playlist(playlist_node* ph);
void play_song(playlist_node** ph);
void play_next(playlist_node** ph,playlist_node** pt);
void play_previous(playlist_node** ph,playlist_node** pt);
void remove_song_from_playlist(playlist_node** ph,playlist_node** pt,int idx);

#endif
