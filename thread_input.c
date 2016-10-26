#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "types.h"
#include "functions.h"
#include "getch.h"

static GameData* gd;
static Point cursor;

static char input_timeout(float seconds) {
    fd_set set;
    struct timeval timeout;
    char input;
    Point cursor;

    /* Initialize the file descriptor set. */
    FD_ZERO(&set);
    FD_SET(0, &set);

    /* Initialize the timeout data structure. */
    int useconds = (seconds / 1000000) + 0.5;
    timeout.tv_sec = 0;
    timeout.tv_usec = useconds;

    /* `select()' returns 0 if timeout, 1 if input available, -1 if error. */
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

static int parse_input(char* input, GameData* gd) {
    switch (*input) {
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
            printf("qveet\n");
            return 0;
            break;
    }
    gd->board[cursor.x][cursor.y] = 'X';
    return 1;
}

void* init_input(void* gd_v) {
    GameData* gd = (GameData*) gd_v;
    int i = 0;
    while (gd->keep_running) {
        i++;
        char input = getch();
        /*
        char input = input_timeout(frame_interval);
        */
        gd->keep_running = parse_input(&input, gd);
    }
    return NULL;
}
