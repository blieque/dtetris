#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#define __USE_POSIX199309 // setting `_POSIX_C_SOURCE' didne work.
#include <time.h>
#undef __USE_POSIX199309

#include "types.h"
#include "functions.h"
//#include "getch.h"
#include "thread_input.h"

static GameData* gd;
static Point cursor;
static struct termios termios_old;
static struct termios termios_new;

/*
static char input_timeout(float seconds) {
    fd_set set;
    struct timeval timeout;
    char input;
    Point cursor;

    // Initialize the file descriptor set.
    FD_ZERO(&set);
    FD_SET(0, &set);

    // Initialize the timeout data structure.
    int useconds = (seconds / 1000000) + 0.5;
    timeout.tv_sec = 0;
    timeout.tv_usec = useconds;

    // `select()' returns 0 if timeout, 1 if input available, -1 if error.
    int data_inputted = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
    if (data_inputted) {
        read(FD_SETSIZE, &input, 1);
    }

    return input;
}

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
            cursor.x--;
            //key_left();
            break;
        case 'j':
        case 'J':
            cursor.y++;
            //key_down();
            break;
        case 'k':
        case 'K':
            cursor.y--;
            //key_up();
            break;
        case 'l':
        case 'L':
            cursor.x++;
            //key_right();
            break;
        case 'q':
        case 'Q':
            return 0;
            break;
    }
    gd->board[cursor.x][cursor.y] = 'X';
    return 1;
}

static void loop_input(GameData* gd) {
    struct timespec frame_delay;
    frame_delay.tv_sec = 0;
    frame_delay.tv_nsec = 500000000;

    while (gd->keep_running) {
        const int buffer_size = 10;
        char buffer[buffer_size];

        const int sequence_buffer_size = 3;
        char sequence_buffer[sequence_buffer_size];
        int sequence_buffer_count = 0;

        int parsing_sequence;

        while (fgets(buffer, sizeof(buffer) + 1, stdin) != NULL) {
            for (int i = 0; i < buffer_size; i++) {
                if (buffer[i] == 0) break;
                if (buffer[i] == 27) parsing_sequence = 1;

                if (!parsing_sequence) {
                    if (buffer[i] == 27) {
                        parsing_sequence = 1;
                    } else {
                        handle_input(buffer[i]);
                    }
                } else {
                    if (sequence_buffer_count < sequence_buffer_size) {
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
                        // check sequence buffer for patters
                        if (strcmp(sequence_buffer, "[A")) {
                            printf("ayy\n");
                        }

                        parsing_sequence = 0;
                    }
                }
            }
        }

        nanosleep(&frame_delay, NULL);
    }
}

void* init_input(void* gd_v) {
    gd = (GameData*) gd_v;

    tcgetattr(0, &termios_old);
    termios_new = termios_old;
    termios_new.c_lflag &= ~ICANON;
    termios_new.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &termios_new);

    loop_input(gd);

    tcsetattr(0, TCSANOW, &termios_old);

    return NULL;
}
