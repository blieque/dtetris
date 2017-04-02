#include <stdio.h>

#ifndef __USE_POSIX
    #define __USE_POSIX // setting `_POSIX_C_SOURCE' didne work.
#endif
#include <signal.h>
#undef __USE_POSIX

#include <string.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <math.h>

#include "types.h"
#include "functions.h"

#include "dtetris.h"
#include "thread_input.h"
#include "thread_rendering.h"

static struct winsize w;
static GameData gd;

void game_data_setup() {
    gd.keep_running = 1;
    gd.frame_rate = 2;
    gd.frame_interval = 1 / gd.frame_rate;
    gd.width = fmax(fmin(w.ws_col / 2, 100), 10);
    gd.height = fmax(fmin(w.ws_row, 100), 10);
    gd._board_data = alloc_board(&gd.board, gd.width, gd.height);
}

void game_data_free() {
    free_board(&gd.board, gd._board_data);
}

void interrupt_handler() {
    gd.keep_running = 0;
    game_data_free();
    console_reset();
    printf("y u gotta be so rude?\n");
    exit(1);
}

void console_setup() {
    // get and set some terminal properties
    setvbuf(stdin, NULL, _IONBF, 0);
    ioctl(0, TIOCGWINSZ, &w);

    // set up ctrl-c handler to return to the normal screen
    struct sigaction sa_interrupt;
    memset(&sa_interrupt, 0, sizeof(struct sigaction));
    sa_interrupt.sa_handler = interrupt_handler;
    sa_interrupt.sa_flags = 0;

    sigaction(SIGINT, &sa_interrupt, NULL);
    sigaction(SIGTERM, &sa_interrupt, NULL);

    // move to alternate screen
    printf("\x1B[?1049h");
    // clear screen and move the cursor to the top left
    printf("\x1B[0;0H");
}

void console_reset() {
    // undo some terminal settings
    //setvbuf(stdin, NULL, _IOLBF, 0);

    // move out of alternate screen
    printf("\x1B[?1049l");
}

int main() {
    console_setup();
    game_data_setup();

    pthread_t thread_input;
    pthread_t thread_rendering;
    pthread_create(&thread_input, NULL, init_input, (void*) &gd);
    pthread_create(&thread_rendering, NULL, init_rendering, (void*) &gd);
    pthread_join(thread_input, NULL);

    game_data_free();
    console_reset();

    return 0;
}
