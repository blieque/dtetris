#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

int input_timeout(int filedes, float seconds) {
    int useconds = round_float(seconds * 1000000);

    fd_set set;
    struct timeval timeout;

    /* Initialize the file descriptor set. */
    FD_ZERO (&set);
    FD_SET (filedes, &set);

    /* Initialize the timeout data structure. */
    timeout.tv_sec = 0;
    timeout.tv_usec = useconds;

    /* select returns 0 if timeout, 1 if input available, -1 if error. */
    return select (FD_SETSIZE, &set, NULL, NULL, &timeout);
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

void parse_input(char input) {
    switch (input) {
        case 'h':
        case 'H':
            key_left();
            break;
        case 'j':
        case 'J':
            key_down();
            break;
        case 'k':
        case 'K':
            key_up();
            break;
        case 'l':
        case 'L':
            key_right();
            break;
        case 'q':
        case 'Q':
            printf("qveet\n");
            exit(1);
            break;
    }
}
