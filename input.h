#include "getch.h"

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

void *init_input(void *arg) {
    while (1) {
        char input = getch();
        if (input == 'q' || input == 'Q') {
            return 0;
        }
        parse_input(&input);
    }
}
