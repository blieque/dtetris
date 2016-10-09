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

void parse_input(char *input) {
    switch (*input) {
        case 'h':
        case 'H':
            printf("\e[1D");
            //key_left();
            break;
        case 'j':
        case 'J':
            printf("\e[1B");
            //key_down();
            break;
        case 'k':
        case 'K':
            printf("\e[1A");
            //key_up();
            break;
        case 'l':
        case 'L':
            printf("\e[1C");
            //key_right();
            break;
        case 'q':
        case 'Q':
            printf("qveet\n");
            exit(1);
            break;
    }
}

void *init_input(void *gd) {
    int i = 0;
    while (i < 10) {
        i++;
        char input = getch();
        /*
        char input = input_timeout(frame_interval);
        */
        if (input == 'q' || input == 'Q') {
            return 0;
        }
        parse_input(&input);
        sleep(1);
    }
    return NULL;
}
