#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include <stdlib.h>
#include <math.h>

typedef struct game_data game_data_t;
struct game_data {
    int keep_running;
    float frame_rate;
    float frame_interval;
    int width;
    int height;
    char** board;
};

#include "functions.h"
#include "input.h"
#include "rendering.h"

struct winsize w;
game_data_t gd;

void console_reset() {
    // undo some terminal settings
    setlinebuf(stdin);

    // move out of alternate screen
    printf("\e[?1049l");
}

void interrupt_handler() {
    console_reset();
    printf("lolnope\n");
    exit(1);
}

void console_setup() {
    // get and set some terminal properties
    ioctl(0, TIOCGWINSZ, &w);
    setbuf(stdin, NULL);

    // move to alternate screen
    printf("\e[?1049h");

    // set up ctrl-c handler to return to the normal screen
    signal(SIGINT, interrupt_handler);
}

void game_data_setup() {
    gd.keep_running = 1;
    gd.frame_rate = 2;
    gd.frame_interval = 1 / gd.frame_rate;
    gd.width = fmax(fmin(w.ws_col / 2, 100), 10);
    gd.height = fmax(fmin(w.ws_row, 100), 10);
    alloc_board(gd.board, gd.width, gd.height);
}

void game_data_free() {
    free_board(&gd.board);
}

int main() {
    //console_setup();
    game_data_setup();

    printf("%i\n", gd.width);

    /*
    pthread_t pth_input;
    pthread_t pth_rendering;
    pthread_create(&pth_input, NULL, init_input, (void*) &gd);
    pthread_create(&pth_rendering, NULL, init_rendering, (void*) &gd);

    pthread_join(pth_input, NULL);
    pthread_cancel(pth_rendering);
    */

    gd.board[0][1] = 'd';
    printf("%c", gd.board[0][1]);
    printf("%c", gd.board[0][2]);
    /*
    */

    for (int i = 0; i < gd.width; i++) {
        free(gd.board[i]);
    }
    free(gd.board);

    //game_data_free();
    //console_reset();

    return 0;
}
