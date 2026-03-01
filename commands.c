#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

void add_to_command_history(const char* cmd){
    FILE* f = fopen("commands.log","a");
    if(!f) return;
    fprintf(f,"%s\n", cmd);
    fclose(f);
}

void show_command_history(){
    FILE* f = fopen("commands.log","r");
    if(!f){ printf("No commands yet.\n"); return; }
    char line[256];
    int i = 1;
    while(fgets(line,sizeof(line),f)){
        line[strcspn(line,"\n")] = 0;
        printf("%d. %s\n", i++, line);
    }
    fclose(f);
}
