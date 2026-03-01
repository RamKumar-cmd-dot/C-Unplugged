#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "songs.h"

song* song_library_head = NULL;
int max_song_id = 0;

void trim_us(char* s){
    int i=0;
    while(s[i]){
        if(s[i]=='_') s[i] = ' ';
        i++;
    }
}

int load_song_library(const char* filename) {
    FILE* f = fopen(filename,"r");
    if(!f) return 0;
    char line[256];
    int count = 0;
    while(fgets(line, sizeof(line), f)) {
        line[strcspn(line,"\n")] = 0;
        if(strlen(line)==0) continue;
        char s_id[16] = {0}, name[80] = {0}, art[80] = {0}, dur[16] = {0};
        int i=0,j=0,k=0,m=0,p=0;
        while(line[i] && line[i] != '|') s_id[j++] = line[i++];
        s_id[j] = 0;
        if(line[i] == '|') i++;
        while(line[i] && line[i] != '|') name[k++] = line[i++];
        name[k] = 0;
        if(line[i] == '|') i++;
        while(line[i] && line[i] != '|') art[m++] = line[i++];
        art[m] = 0;
        if(line[i] == '|') i++;
        while(line[i]) dur[p++] = line[i++];
        dur[p] = 0;
        song* s = malloc(sizeof(song));
        if(!s)  continue; 
        s->song_id = atoi(s_id);
        strncpy(s->song_name, name, sizeof(s->song_name)-1);
        s->song_name[sizeof(s->song_name)-1] = 0;
        strncpy(s->artist_name, art, sizeof(s->artist_name)-1);
        s->artist_name[sizeof(s->artist_name)-1] = 0;
        strncpy(s->duration, dur, sizeof(s->duration)-1);
        s->duration[sizeof(s->duration)-1] = 0;
        s->next = NULL;
        if(!song_library_head) song_library_head = s;
        else{
            song* t = song_library_head;
            while(t->next) t = t->next;
            t->next = s;
        }
        if(s->song_id > max_song_id) max_song_id = s->song_id;
        count++;
    }
    fclose(f);
    return count;
}

void list_all_songs() {
    song* s = song_library_head;
    if(!s){ printf("No songs in library\n"); return; }
    int idx = 1;
    while(s){
        char name[80], ar[80];
        strcpy(name, s->song_name);
        strcpy(ar, s->artist_name);
        trim_us(name);
        trim_us(ar);
        printf("%d. %s -- %s [%s]\n", idx, name, ar, s->duration);
        s = s->next;
        idx++;
    }
}

song* find_song_by_id(int id) {
    song* s = song_library_head;
    while(s){
        if(s->song_id == id) return s;
        s = s->next;
    }
    return NULL;
}

void add_new_song_to_library(){
    char name[80], artist[80];
    int mm, ss;
    printf("Enter song name: ");
    if(!fgets(name,sizeof(name),stdin)) return;
    name[strcspn(name,"\n")] = 0;
    printf("Enter artist name: ");
    if(!fgets(artist,sizeof(artist),stdin)) return;
    artist[strcspn(artist,"\n")] = 0;
    printf("Enter duration minutes: ");
    if(scanf("%d",&mm)!=1){ while(getchar()!='\n'); printf("Invalid input\n"); return; }
    printf("Enter duration seconds: ");
    if(scanf("%d",&ss)!=1){ while(getchar()!='\n'); printf("Invalid input\n"); return; }
    while(getchar()!='\n');
    max_song_id++;
    int new_id = max_song_id;
    song* s = malloc(sizeof(song));
    if(!s){ printf("Memory error\n"); return; }
    s->song_id = new_id;
    strncpy(s->song_name, name, sizeof(s->song_name)-1);
    s->song_name[sizeof(s->song_name)-1] = 0;
    strncpy(s->artist_name, artist, sizeof(s->artist_name)-1);
    s->artist_name[sizeof(s->artist_name)-1] = 0;
    snprintf(s->duration, sizeof(s->duration), "%02d:%02d", mm, ss);
    s->next = NULL;
    if(song_library_head==NULL) song_library_head = s;
    else{
        song* t = song_library_head;
        while(t->next) t = t->next;
        t->next = s;
    }
    FILE* f = fopen("songs.txt","a");
    if(f){
        fprintf(f,"%d|%s|%s|%02d:%02d\n", new_id, name, artist, mm, ss);
        fclose(f);
    }
    printf("New song added successfully.\n");
}
