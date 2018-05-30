#include <stdio.h>

#ifndef __USE_POSIX
    // Setting `_POSIX_C_SOURCE' didn't work.
    #define __USE_POSIX
#endif
#include <signal.h>
#undef __USE_POSIX

#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pthread.h>

#include "types.h"
#include "util.h"
#include "board.h"

#include "threads/game.h"
#include "threads/input.h"
#include "threads/rendering.h"

static struct winsize w;
static GameData gd;
static struct termios termios_new;
static struct termios termios_old;

/*
 * Set various properties of the game data object for reference during
 * operation.
 */
void game_data_setup() {
    pthread_mutex_init(&gd.mutex, NULL);

    gd.keep_running = true;

    gd.frame_rate = 2;
    gd.frame_interval = 1 / gd.frame_rate;

    gd.width = max(min(w.ws_col / 2, 100), 10);
    gd.height = max(min(w.ws_row, 100), 10);
    gd.cursor.x = 0;
    gd.cursor.y = gd.height - 1;
    gd.board = Board_new(gd.width, gd.height);
}

/*
 * Free up the memory previously allocated for storage of the game board.
 */
void game_data_free() {
    Board_free(&gd.board);
}

// Prototype to avoid errors.
void interrupt_handler();
/*
 * Configure various terminal settings for game operation.
 */
void console_setup() {
    // Get terminal dimensions (and other stuff).
    ioctl(0, TIOCGWINSZ, &w);

    // Turn off buffering for the `stdin' and `stdout' streams.
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Disable character echoing.
    tcgetattr(0, &termios_old);
    termios_new = termios_old;
    termios_new.c_lflag &= ~ICANON;
    termios_new.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &termios_new);

    // Set up ^C handler to return to the shell.
    struct sigaction sa_interrupt;
    memset(&sa_interrupt, 0, sizeof(struct sigaction));
    sa_interrupt.sa_handler = interrupt_handler;
    sa_interrupt.sa_flags = 0;

    sigaction(SIGINT, &sa_interrupt, NULL);
    sigaction(SIGTERM, &sa_interrupt, NULL);

    // Move to alternate screen.
    printf("\033[?1049h");
    // Clear screen and move the cursor to the top left.
    printf("\033[0;0H");
}

/*
 * Reverse changes made to terminal operation and return to the shell.
 */
void console_reset() {
    // Return to user shell.
    printf("\033[?1049l");

    // Undo some terminal settings.jn
    setvbuf(stdin, NULL, _IOLBF, 0);
    setvbuf(stdout, NULL, _IOLBF, 0);
    tcsetattr(0, TCSANOW, &termios_old);
}

/*
 * Handler function called when operation is interrupted. This allows us to
 * tidily exit the program and question the user's morals.
 */
void interrupt_handler() {
    gd.keep_running = false;
    game_data_free();
    console_reset();
    printf("y u gotta be so rude?\n");
    exit(1);
}

int main() {
    console_setup();
    game_data_setup();

    // Create execution threads.

    pthread_t thread_game;
    pthread_t thread_input;
    pthread_t thread_rendering;

    pthread_create(&thread_game, NULL, init_game, &gd);
    pthread_create(&thread_input, NULL, init_input, &gd);
    pthread_create(&thread_rendering, NULL, init_rendering, &gd);

    //pthread_join(thread_game, NULL);
    pthread_join(thread_input, NULL);
    pthread_join(thread_rendering, NULL);

    game_data_free();
    console_reset();

    return 0;
}
