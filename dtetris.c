#include <stdio.h>

#ifndef __USE_POSIX
    #define __USE_POSIX // setting `_POSIX_C_SOURCE' didne work.
#endif
#include <signal.h>
#undef __USE_POSIX

#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pthread.h>
#include <math.h>

#include "types.h"
#include "functions.h"

#include "dtetris.h"
#include "thread_input.h"
#include "thread_rendering.h"

static struct winsize w;
static GameData gd;
static struct termios termios_new;
static struct termios termios_old;

void game_data_setup() {
    gd.keep_running = true;
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
    gd.keep_running = false;
    game_data_free();
    console_reset();
    printf("y u gotta be so rude?\n");
    exit(1);
}

void console_setup() {
    // get terminal dimensions (and other stuff) and place it in `w'
    ioctl(0, TIOCGWINSZ, &w);

    // turn off buffering for the `stdin' and `stdout' streams
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // disable character echoing
    tcgetattr(0, &termios_old);
    termios_new = termios_old;
    termios_new.c_lflag &= ~ICANON;
    termios_new.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &termios_new);

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
    // move out of alternate screen
    printf("\x1B[?1049l");

    // undo some terminal settings
    setvbuf(stdin, NULL, _IOLBF, 0);
    setvbuf(stdout, NULL, _IOLBF, 0);
    tcsetattr(0, TCSANOW, &termios_old);
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
