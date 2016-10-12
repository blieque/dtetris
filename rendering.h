#include <stdio.h>
#include <unistd.h>

//#include "functions.h"

char** last_board;

void move_to(int x, int y) {
    if (x > 0 && y > 0) {
        x++;
        y++;
        printf("\e[%d;%dH", x, y);
    }
}

void* init_rendering(void* gd_p) {
    game_data_t* gd = (game_data_t*) gd_p;
    alloc_board(last_board, gd->width, gd->height);
    while (gd->keep_running) {
        printf("##\e[2D");
        sleep(1);
    }
    return NULL;
}
