#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playlist.h"
#include "album.h"
#include "songs.h"

playlist_node* playlist_head = NULL;
playlist_node* playlist_tail = NULL;
playlist_node* current_play = NULL;

void view_playlist(playlist_node* ph){
    if(!ph){ printf("Playlist empty.\n"); return; }
    int idx = 1;
    playlist_node* cur = ph;
    while(cur){
        song* s = cur->song_ptr;
        if(s){
            char nm[80], ar[80];
            strcpy(nm, s->song_name);
            strcpy(ar, s->artist_name);
            trim_us(nm);
            trim_us(ar);
            printf("%d. %s -- %s [%s]\n", idx, nm, ar, s->duration);
        }
        idx++;
        cur = cur->next;
    }
}

void add_song_to_playlist(){
    list_all_songs();
    printf("Select song: ");
    int x;
    if(scanf("%d",&x)!=1){
        while(getchar()!='\n');
        printf("Invalid\n");
        return;
    }
    while(getchar()!='\n');

    int idx = 1;
    song* s = song_library_head;
    while(s && idx < x){
        s = s->next;
        idx++;
    }
    if(!s){
        printf("Invalid song index\n");
        return;
    }

    playlist_node* node = malloc(sizeof(playlist_node));
    if(!node){
        printf("Memory error\n");
        return;
    }

    node->song_ptr = s;
    node->next = NULL;
    node->prev = playlist_tail;

    if(playlist_tail) playlist_tail->next = node;
    else playlist_head = node;

    playlist_tail = node;

    printf("Song added to playlist\n");
}


void add_album_to_playlist(int album_idx,playlist_node** ph,playlist_node** pt){
    album* a = album_head_ptr;
    int i = 1;
    while(a){
        if(i == album_idx) break;
        i++;
        a = a->next;
    }
    if(!a){ printf("Album not found\n"); return; }
    album_song* as = a->song_head;
    while(as){
        song* s = find_song_by_id(as->song_id);
        if(s){
            playlist_node* node = malloc(sizeof(playlist_node));
            if(!node){ printf("Memory error\n"); return; }
            node->song_ptr = s;
            node->next = NULL;
            node->prev = *pt;
            if(*pt) (*pt)->next = node;
            else *ph = node;
            *pt = node;
        }
        as = as->next;
    }
    printf("All songs from album added to playlist\n");
}
void play_song(playlist_node** ph){
    if(*ph == NULL){ 
        printf("Playlist empty.\n"); 
        return; }
    if(current_play == NULL) current_play = *ph;
    song* s = current_play->song_ptr;
    char nm[80], ar[80];
    strcpy(nm, s->song_name);
    strcpy(ar, s->artist_name);
    trim_us(nm);
    trim_us(ar);
    printf("Playing: %s -- %s [%s]\n", nm, ar, s->duration);
}
void play_next(playlist_node** ph,playlist_node** pt){
    if(!*ph){ printf("Playlist empty.\n"); return; }
    if(!current_play) current_play = *ph;
    else if(current_play->next) current_play = current_play->next;
    else current_play = *ph;
    play_song(ph);
}
void play_previous(playlist_node** ph,playlist_node** pt){
    if(!*ph){ printf("Playlist empty.\n"); return; }
    if(!current_play) current_play = *pt;
    else if(current_play->prev) current_play = current_play->prev;
    else current_play = *pt;
    play_song(ph);
}
void remove_song_from_playlist(playlist_node** ph,playlist_node** pt,int idx){
    if(!*ph){ printf("Playlist empty.\n"); return; }
    playlist_node* cur = *ph;
    int i = 1;
    while(cur){
        if(i == idx){
            if(cur->prev) cur->prev->next = cur->next;
            else *ph = cur->next;
            if(cur->next) cur->next->prev = cur->prev;
            else *pt = cur->prev;
            if(current_play == cur) current_play = cur->next ? cur->next : *ph;
            free(cur);
            printf("Song removed from playlist\n");
            return;
        }
        cur = cur->next;
        i++;
    }
    printf("Song not found in playlist\n");
}