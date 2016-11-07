#include <stdio.h>
#include <unistd.h>

#include "types.h"
#include "functions.h"
#include "thread_rendering.h"

static GameData* gd;
static char** last_board;
static char* _last_board_data;

/*
static void move_to(int x, int y) {
    if (x > 0 && y > 0) {
        x++;
        y++;
        printf("\e[%d;%dH", x, y);
    }
}
*/

static void render_diff() {

}

void* init_rendering(void* gd_p) {
    gd = (GameData*) gd_p;
    _last_board_data = alloc_board(&last_board, gd->width, gd->height);
    while (gd->keep_running) {
        render_diff();
        sleep(1);
    }
    free_board(&last_board, _last_board_data);
    return NULL;
}
