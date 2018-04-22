#include "input.h"
#include "../types.h"
#include "../board.h"

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

// Setting `_POSIX_C_SOURCE' didn't work.
#define __USE_POSIX199309
    #include <time.h>
#undef __USE_POSIX199309

static GameData* gd;
static Point* cursor;

static void key_left() {
    if (cursor->x > 0) cursor->x--;
}
static void key_down() {
    if (cursor->y > 0) cursor->y--;
}
static void key_up() {
    if (cursor->y < gd->height - 1) cursor->y++;
}
static void key_right() {
    if (cursor->x < gd->width - 1) cursor->x++;
}

static int handle_input(char input) {
    pthread_mutex_lock(&gd->mutex);
    gd->board.cell[cursor->x][cursor->y] = 0;

    switch (tolower(input)) {
        case 'h':
        case 'a':
            key_left();
            break;
        case 'j':
        case 's':
            key_down();
            break;
        case 'k':
        case 'w':
            key_up();
            break;
        case 'l':
        case 'd':
            key_right();
            break;
        case 'q':
            gd->keep_running = false;
            return 0;
            break;
    }

    gd->board.cell[cursor->x][cursor->y] = 1;
    pthread_mutex_unlock(&gd->mutex);

    return 1;
}

static bool stdin_has_input() {
    // Tell select to immediately return if there is no input, rather than
    // waiting until there is.
    struct timeval select_delay;
    select_delay.tv_sec = 0;
    select_delay.tv_usec = 0;

    // `stdin' file descriptor set for `select()'
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);

    // Return true if there is input waiting to be processed, else return false
    if (select(1, &set, NULL, NULL, &select_delay)) {
        return true;
    } else {
        return false;
    }
}

/*
 * Repeatedly check for input on `stdin', and process any that is present.
 */
static void loop_input() {
    // Delay between input processing loops.
    struct timespec frame_delay;
    frame_delay.tv_sec = 0;
    frame_delay.tv_nsec = 20000000;

    // Buffer to store input from `stdin'. Seeing as we process input several
    // times a second, the buffer needn't be big at all.
    const int buffer_size = 10;
    char buffer[buffer_size];

    // Buffer to store escape sequences encountered in `stdin'. Escape sequences
    // can be much bigger than will fit in this buffer, but any that are bigger
    // are ones we don't care about.
    const int sequence_buffer_size = 3;
    const int sequence_buffer_limit = sequence_buffer_size - 1;
    char sequence_buffer[sequence_buffer_size];
    int sequence_buffer_count;

    // Boolean to track whether or not an escape sequence is being parsed.
    bool parsing_sequence;

    // Main input processing loop.
    while (gd->keep_running) {
        parsing_sequence = false;

        while (stdin_has_input()) {
            int chars_read = read(0, buffer, buffer_size);

            for (int i = 0; i < chars_read; i++) {
                // End of buffer?
                if (buffer[i] == '\0') break;

                // Detect escape character.
                if (buffer[i] == '\033') {
                    // Reset values.
                    sequence_buffer_count = 0;
                    parsing_sequence = true;
                    memset(sequence_buffer, 0, sequence_buffer_size);
                    // Stops the escape character being buffered itself.
                    continue;
                }

                if (!parsing_sequence) {
                    if (buffer[i] == '\033') {
                        parsing_sequence = true;
                    } else {
                        handle_input(buffer[i]);
                    }
                } else {
                    if (sequence_buffer_count < sequence_buffer_limit) {
                        sequence_buffer[sequence_buffer_count] = buffer[i];
                        sequence_buffer_count++;
                    }
                    /*
                     * ANSI escape sequences end in an upper- or lowercase
                     * Latin letter, so we'll end sequence parsing mode when we
                     * see one.
                     */
                    if ((buffer[i] >= 'A' && buffer[i] <= 'Z') ||
                        (buffer[i] >= 'a' && buffer[i] <= 'z')) {

                        // Check sequence buffer for arrow key sequences.
                        if (strcmp(sequence_buffer, "[A") == 0) {
                            handle_input('k');
                        } else if (strcmp(sequence_buffer, "[B") == 0) {
                            handle_input('j');
                        } else if (strcmp(sequence_buffer, "[C") == 0) {
                            handle_input('l');
                        } else if (strcmp(sequence_buffer, "[D") == 0) {
                            handle_input('h');
                        }

                        parsing_sequence = false;
                    }
                }
            }
        }

        // Delay before checking for input again.
        nanosleep(&frame_delay, NULL);
    }
}

/*
 * Initialise the input thread.
 */
void* init_input(void* gd_v) {
    gd = (GameData*) gd_v;
    cursor = &gd->cursor;

    loop_input();

    return NULL;
}
