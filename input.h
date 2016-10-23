#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#include "getch.h"

char input_timeout(float seconds) {
    fd_set set;
    struct timeval timeout;
    char input;

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

void key_left() {
    printf("lef'\n");
}
void key_down() {
    printf("daan\n");
}
void key_up() {
    printf("ap\n");
}
void key_right() {
    printf("raight\n");
}

int parse_input(char *input) {
    switch (*input) {
        case 'h':
        case 'H':
            printf("\e[1D\e[1D");
            printf("##");
            printf("\e[1D\e[1D");
            //key_left();
            break;
        case 'j':
        case 'J':
            printf("\e[1B");
            printf("##");
            printf("\e[1D\e[1D");
            //key_down();
            break;
        case 'k':
        case 'K':
            printf("\e[1A");
            printf("##");
            printf("\e[1D\e[1D");
            //key_up();
            break;
        case 'l':
        case 'L':
            printf("\e[1C\e[1C");
            printf("##");
            printf("\e[1D\e[1D");
            //key_right();
            break;
        case 'q':
        case 'Q':
            printf("qveet\n");
            return 0;
            break;
    }
    return 1;
}

void* init_input(void* gd_v) {
    game_data_t* gd = (game_data_t*) gd_v;
    int i = 0;
    while (gd->keep_running) {
        i++;
        char input = getch();
        /*
        char input = input_timeout(frame_interval);
        */
        gd->keep_running = parse_input(&input);
    }
    return NULL;
}
