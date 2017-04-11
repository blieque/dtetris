#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#define __USE_POSIX199309 // setting `_POSIX_C_SOURCE' didne work.
#include <time.h>
#undef __USE_POSIX199309

#include "types.h"
#include "functions.h"
#include "thread_input.h"

static GameData* gd;
static Point cursor;

/*
static void key_left() {
    printf("lef'\n");
}
static void key_down() {
    printf("daan\n");
}
static void key_up() {
    printf("ap\n");
}
static void key_right() {
    printf("raight\n");
}
*/

static int handle_input(char input) {
    switch (input) {
        case 'h':
        case 'H':
        case 'a':
        case 'A':
            cursor.x--;
            printf("leffe\n");
            //key_left();
            break;
        case 'j':
        case 'J':
        case 's':
        case 'S':
            cursor.y++;
            printf("doon\n");
            //key_down();
            break;
        case 'k':
        case 'K':
        case 'w':
        case 'W':
            cursor.y--;
            printf("hup\n");
            //key_up();
            break;
        case 'l':
        case 'L':
        case 'd':
        case 'D':
            cursor.x++;
            printf("aight\n");
            //key_right();
            break;
        case 'q':
        case 'Q':
            gd->keep_running = false;
            return 0;
            break;
    }
    return 1;
}

static bool stdin_has_input() {
    // timeval for the time `select()' will wait for input (0 seconds)
    struct timeval select_delay;
    select_delay.tv_sec = 0;
    select_delay.tv_usec = 0;

    // `stdin' file descriptor set for `select()'
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set);

    // return true if there is input waiting to be processed, else return false
    if (select(1, &set, NULL, NULL, &select_delay)) {
        return true;
    } else {
        return false;
    }
}

static void loop_input() {
    // timespec for the delay between input processing loops
    struct timespec frame_delay;
    frame_delay.tv_sec = 0;
    frame_delay.tv_nsec = 20000000;

    // buffer to store input from `stdin'
    // Seeing as we process input several times a second, the buffer needn't be
    // big at all.
    const int buffer_size = 10;
    char buffer[buffer_size];

    // buffer to store escape sequences encountered in `stdin'
    // Escape sequences can be much bigger than will fit in this buffer, but any
    // that are bigger are ones we don't care about.
    const int sequence_buffer_size = 3;
    const int sequence_buffer_limit = sequence_buffer_size - 1;
    char sequence_buffer[sequence_buffer_size];
    int sequence_buffer_count;

    // boolean to track whether or not an escape sequence is being parsed
    bool parsing_sequence;

    while (gd->keep_running) {
        parsing_sequence = false;

        while (stdin_has_input()) {
            int chars_read = read(0, buffer, buffer_size);

            for (int i = 0; i < chars_read; i++) {
                if (buffer[i] == 0) break;
                if (buffer[i] == 27) {
                    sequence_buffer_count = 0;
                    parsing_sequence = true;
                    memset(sequence_buffer, 0, sequence_buffer_size);
                    continue; // stops the escape character being buffered
                }

                if (!parsing_sequence) {
                    if (buffer[i] == 27) {
                        parsing_sequence = true;
                    } else {
                        handle_input(buffer[i]);
                    }
                } else {
                    if (sequence_buffer_count < sequence_buffer_limit) {
                        sequence_buffer[sequence_buffer_count] = buffer[i];
                        sequence_buffer_count++;
                    }
                    /**
                     * ANSI escape sequences end in an upper- or lowercase
                     * Latin letter, so we'll end sequence parsing mode when we
                     * see one.
                     */
                    if ((buffer[i] > 64 && buffer[i] < 91) ||
                        (buffer[i] > 96 && buffer[i] < 123)) {
                        // check sequence buffer for patterns
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

        nanosleep(&frame_delay, NULL);
    }
}

void* init_input(void* gd_v) {
    gd = (GameData*) gd_v;

    loop_input();

    return NULL;
}
