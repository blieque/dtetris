#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define __USE_POSIX199309 // setting `_POSIX_C_SOURCE' didne work.
#include <time.h>
#undef __USE_POSIX199309

#include "types.h"
#include "functions.h"
#include "thread_rendering.h"

static GameData* gd;
static char** last_board;
static char* _last_board_data;

static void move_to(int x, int y) {
    if (x > 0 && y > 0) {
        x++;
        x *= 2;
        y++;
        printf("\x1B[%d;%dH", y, x);
    }
}

static void render_diff() {
    for (int i = 0; i < gd->width; i++) {
        for (int j = 0; j < gd->height; j++) {
            if (gd->board[i][j] != last_board[i][j]) {
                move_to(i, j);
                printf("%c%c", gd->board[i][j], gd->board[i][j]);
            }
        }
    }
}

void* init_rendering(void* gd_p) {
    gd = (GameData*) gd_p;
    _last_board_data = alloc_board(&last_board, gd->width, gd->height);

    struct timespec frame_delay;
    frame_delay.tv_sec = 0;
    frame_delay.tv_nsec = 200000000;
    while (gd->keep_running) {
        render_diff();
        memcpy(_last_board_data,
               gd->_board_data,
               gd->width * gd->height * sizeof(char));
        nanosleep(&frame_delay, NULL);
        //sleep(1);
    }

    free_board(&last_board, _last_board_data);
    return NULL;
}
