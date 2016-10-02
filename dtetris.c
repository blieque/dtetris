#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getch.h"

void key_left() {

}
void key_down() {

}
void key_up() {

}
void key_right() {

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

int main() {
    printf("\e[?1049h");

    while (1) {
        char input = getch();
        parse_input(input);
    }

    /*  */
    printf("\e[?1049l");
    return 0;
}
