#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include <stdlib.h>
#include <math.h>

#include "functions.h"
#include "input.h"
#include "rendering.h"

struct game_data {
    float frame_rate;
    float frame_interval;
    int width;
    int height;
    char** board;
};

struct winsize w;
struct game_data gd;

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
    gd.frame_rate = 2;
    gd.frame_interval = 1 / gd.frame_rate;
    gd.width = fmax(fmin(w.ws_col / 2, 100), 10);
    gd.height = fmax(fmin(w.ws_row, 100), 10);

    gd.board = malloc(gd.width * sizeof(char*));
    for (int i = 0; i < gd.height; i++) {
        gd.board[i] = malloc(gd.height * sizeof(char));
    }
    printf("%d\n", gd.board[2][0]);
}

void game_data_free() {
    for (int i = 0; i < gd.height; i++) {
        free(gd.board[i]);
    }
    free(gd.board);
}

int main() {
    //console_setup();
    game_data_setup();

    pthread_t pth_input;
    pthread_t pth_rendering;
    pthread_create(&pth_input, NULL, init_input, &gd);
    pthread_create(&pth_rendering, NULL, init_rendering, &gd);

    pthread_join(pth_input, NULL);
    pthread_cancel(pth_rendering);

    game_data_free();
    //console_reset();

    return 0;
}
