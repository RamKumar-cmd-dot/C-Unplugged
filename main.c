#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "songs.h"
#include "album.h"
#include "playlist.h"
#include "commands.h"
void create_library(){
    load_song_library("songs.txt");
    load_albums_from_file("albums.txt");
}
void freeup_all(){
    save_albums_to_file("albums.txt");
    playlist_node* p = playlist_head;
    while(p){
        playlist_node* t = p;
        p = p->next;
        free(t);}
    playlist_head = playlist_tail = current_play = NULL;
    album* a = album_head_ptr;
    while(a){
        album_song* as = a->song_head;
        while(as){
            album_song* t = as;
            as = as->next;
            free(t);}
        album* ta = a;
        a = a->next;
        free(ta);}
    album_head_ptr = NULL;
    song* s = song_library_head;
    while(s){
        song* ts = s;
        s = s->next;
        free(ts);}
    song_library_head = NULL;}
int main(){
    create_library();
    int val;
    while(1){
        printf("\nC-Unplugged Menu\n");
        printf("1. List albums\n");
        printf("2. List songs\n");
        printf("3. View album\n");
        printf("4. Create album\n");
        printf("5. Delete album\n");
        printf("6. Add song to album\n");
        printf("7. Delete song from album\n");
        printf("8. Create new song\n");
        printf("9. Add song to playlist\n");
        printf("10. Add album to playlist\n");
        printf("11. View playlist\n");
        printf("12. Play current song\n");
        printf("13. Play next\n");
        printf("14. Play previous\n");
        printf("15. Remove song from playlist\n");
        printf("16. Show command history\n");
        printf("17. Exit\n");
        printf("C-Unplugged >> Choose command index  : ");
        if(scanf("%d",&val)!=1){ while(getchar()!='\n'); continue; }
        while(getchar()!='\n');
        if(val==1){
            print_all_albums();
            add_to_command_history("List albums");
        } else if(val==2){
            list_all_songs();
            add_to_command_history("List songs");
        } else if(val==3){
            print_all_albums();
            printf("Select album: ");
            int x;
            if(scanf("%d",&x)!=1){
                while(getchar()!='\n');
                continue; }
            while(getchar()!='\n');
            show_album(x);
            add_to_command_history("View album");
        } else if(val==4){
            char name[80];
            printf("Enter album name: ");
            if(!fgets(name,sizeof(name),stdin)) continue;
            name[strcspn(name,"\n")] = 0;
            album* a = create_album(name);
            add_album(a);
            printf("Album '%s' created\n", name);
            add_to_command_history("Create album");
        } else if(val==5){
            print_all_albums();
            printf("Select album to delete: ");
            int x;
            if(scanf("%d",&x)!=1){ while(getchar()!='\n'); continue; }
            while(getchar()!='\n');
            delete_album(x);
            add_to_command_history("Delete album");
        } else if(val==6){
            print_all_albums();
            printf("Select album: ");
            int x;
            if(scanf("%d",&x)!=1){
                while(getchar()!='\n');
                continue; }
            while(getchar()!='\n');
            add_song_to_album(x);
            add_to_command_history("Add song to album");
        } else if(val==7){
            print_all_albums();
            printf("Select album: ");
            int x;
            if(scanf("%d",&x)!=1){
                while(getchar()!='\n');
                continue; }
            while(getchar()!='\n');
            show_album(x);
            printf("Select song to delete: ");
            int y;
            if(scanf("%d",&y)!=1){ while(getchar()!='\n'); continue; }
            while(getchar()!='\n');
            delete_song_from_album(x,y);
            add_to_command_history("Delete song from album");
        } else if(val==8){
            add_new_song_to_library();
            add_to_command_history("Create new song");
        }
            else if(val==9){
            add_song_to_playlist();
            add_to_command_history("Add song to playlist");
            }
         else if(val==10){
            print_all_albums();
            printf("Select album: ");
            int x;
            if(scanf("%d",&x)!=1){
                while(getchar()!='\n');
                continue; }
            while(getchar()!='\n');
            add_album_to_playlist(x,&playlist_head,&playlist_tail);
            add_to_command_history("Add album to playlist");
        } else if(val==11){
            view_playlist(playlist_head);
            add_to_command_history("View playlist");
        } else if(val==12){
            play_song(&playlist_head);
            add_to_command_history("Play current");
        } else if(val==13){
            play_next(&playlist_head,&playlist_tail);
            add_to_command_history("Play next");
        } else if(val==14){
            play_previous(&playlist_head,&playlist_tail);
            add_to_command_history("Play previous");
        } else if(val==15){
            view_playlist(playlist_head);
            printf("Enter playlist song index to remove: ");
            int idx;
            if(scanf("%d",&idx) != 1){ while(getchar()!='\n'); continue; }
            while(getchar()!='\n');
            remove_song_from_playlist(&playlist_head,&playlist_tail,idx);
            add_to_command_history("Remove song from playlist");
        } else if(val==16){
            show_command_history();
            add_to_command_history("Show command history");
        } else if(val==17){
            break;
        } else {
            printf("Unknown option\n");
        }
    }
    freeup_all();
    printf("Good Bye\n");
    return 0;
}
