#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "album.h"
#include "songs.h"
#include "playlist.h"
#include "commands.h"

album* album_head_ptr = NULL;
int max_album_id = 0;

void add_album(album* a) {
    if(!a) return;
    if(album_head_ptr == NULL) album_head_ptr = a;
    else{
        album* t = album_head_ptr;
        while(t->next) t = t->next;
        t->next = a;
    }
}

void load_albums_from_file(const char* filename) {
    FILE* f = fopen(filename,"r");
    if(!f) return;
    char line[256];
    while(fgets(line,sizeof(line),f)) {
        line[strcspn(line,"\n")] = 0;
        if(strlen(line)==0) continue;
        if(strncmp(line,"ALBUM|",6)==0){
            int i=6,j=0,k=0;
            char a_id[16] = {0}, name[80] = {0};
            while(line[i] && line[i] != '|') a_id[j++] = line[i++];
            a_id[j] = 0;
            if(line[i] == '|') i++;
            while(line[i]) name[k++] = line[i++];
            name[k] = 0;
            album* a = malloc(sizeof(album));
            if(!a) continue;
            a->album_id = atoi(a_id);
            strncpy(a->album_name, name, sizeof(a->album_name)-1);
            a->album_name[sizeof(a->album_name)-1] = 0;
            a->song_head = NULL;
            a->next = NULL;
            if(!album_head_ptr) album_head_ptr = a;
            else{
                album* t = album_head_ptr;
                while(t->next) t = t->next;
                t->next = a;
            }
            if(a->album_id > max_album_id) max_album_id = a->album_id;
        } else if(strncmp(line,"SONGREF|",8)==0){
            int i=8,j=0,k=0;
            char a_id[16] = {0}, s_id[16] = {0};
            while(line[i] && line[i] != '|') a_id[j++] = line[i++];
            a_id[j] = 0;
            if(line[i] == '|') i++;
            while(line[i]) s_id[k++] = line[i++];
            s_id[k] = 0;
            int albumid = atoi(a_id);
            int songid = atoi(s_id);
            album* a = album_head_ptr;
            while(a && a->album_id != albumid) a = a->next;
            if(!a) continue;
            album_song* as = malloc(sizeof(album_song));
            if(!as) continue;
            as->song_id = songid;
            as->next = NULL;
            if(!a->song_head) a->song_head = as;
            else{
                album_song* t = a->song_head;
                while(t->next) t = t->next;
                t->next = as;
            }
        }
    }
    fclose(f);
}

void save_albums_to_file(const char* filename) {
    FILE* f = fopen(filename,"w");
    if(!f) return;
    album* a = album_head_ptr;
    while(a){
        fprintf(f,"ALBUM|%d|%s\n", a->album_id, a->album_name);
        album_song* as = a->song_head;
        while(as){
            fprintf(f,"SONGREF|%d|%d\n", a->album_id, as->song_id);
            as = as->next;
        }
        a = a->next;
    }
    fclose(f);
}

void print_all_albums(){
    album* a = album_head_ptr;
    int idx = 1;
    if(!a){ 
        printf("No albums.\n"); 
        return; }
    while(a){
        char name[80];
        strcpy(name, a->album_name);
        trim_us(name);
        printf("%d. %s\n", idx++, name);
        a = a->next;
    }
}

void show_album(int album_idx) {
    album* a = album_head_ptr;
    int i=1;
    while(a){
        if(i == album_idx) break;
        i++;
        a = a->next;
    }
    if(!a){ printf("Album not found\n"); return; }
    album_song* as = a->song_head;
    if(!as){ printf("No songs in album\n"); return; }
    int idx = 1;
    while(as){
        song* s = find_song_by_id(as->song_id);
        if(s){
            char name[80], ar[80];
            strcpy(name, s->song_name);
            strcpy(ar, s->artist_name);
            trim_us(name);
            trim_us(ar);
            printf("%d. %s -- %s [%s]\n", idx, name, ar, s->duration);
        } else {
            printf("%d. Unknown song\n", idx);
        }
        idx++;
        as = as->next;
    }
}

album* create_album(const char* name) {
    album* a = malloc(sizeof(album));
    if(!a) return NULL;
    a->album_id = max_album_id + 1;
    max_album_id++;
    strncpy(a->album_name, name, sizeof(a->album_name)-1);
    a->album_name[sizeof(a->album_name)-1] = 0;
    a->song_head = NULL;
    a->next = NULL;
    return a;
}

void delete_album(int idx) {
    album* cur = album_head_ptr;
    album* prev = NULL;
    int i = 1;
    while(cur){
        if(i == idx){
            if(prev) prev->next = cur->next;
            else album_head_ptr = cur->next;
            album_song* as = cur->song_head;
            while(as){
                album_song* t = as;
                as = as->next;
                free(t);
            }
            free(cur);
            printf("Album deleted\n");
            return;
        }
        prev = cur;
        cur = cur->next;
        i++;
    }
    printf("Album not found\n");
}

void add_song_to_album(int album_idx){
    album* a = album_head_ptr;
    int i = 1;
    while(a){
        if(i == album_idx) break;
        i++;
        a = a->next;
    }
    if(!a){ printf("Album not found\n"); return; }
    list_all_songs();
    printf("Enter song number: ");
    int choice;
    if(scanf("%d", &choice) != 1){ 
        while(getchar() != '\n'); 
        printf("Invalid\n"); 
        return; }
    while(getchar() != '\n');
    int idx = 1;
    song* s = song_library_head;
    while(s && idx < choice){ s = s->next; idx++; }
    if(!s){ 
        printf("Invalid choice\n"); 
        return; }
    album_song* as = malloc(sizeof(album_song));
    if(!as){ 
        printf("Memory error\n"); 
        return; }
    as->song_id = s->song_id;
    as->next = NULL;
    if(!a->song_head) a->song_head = as;
    else{
        album_song* t = a->song_head;
        while(t->next) t = t->next;
        t->next = as;
    }
    printf("Song added to album\n");
}

void delete_song_from_album(int album_idx, int song_idx) {
    album* a = album_head_ptr;
    int i = 1;
    while(a){
        if(i == album_idx) break;
        i++;
        a = a->next;
    }
    if(!a){ 
        printf("Album not found\n"); 
        return; }
    album_song* cur = a->song_head;
    album_song* prev = NULL;
    int j = 1;
    while(cur){
        if(j == song_idx){
            if(prev) prev->next = cur->next;
            else a->song_head = cur->next;
            free(cur);
            printf("Song removed\n");
            return;
        }
        prev = cur;
        cur = cur->next;
        j++;
    }
    printf("Song index not found\n");
}
