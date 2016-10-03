#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

//#include "getch.h"

#include "functions.h"
#include "input.h"

void move_to(int x, int y) {
    if (x > 0 && y > 0) {
        x++;
        y++;
        printf("\e[%d;%dH", x, y);
    }
}

int main() {
    /*

    while (1) {
        char input = getch();
        if (input == 'b') {
            break;
        }
        parse_input(input);
    }

    */

    // pre-flight checks
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    printf("%d by %d\n", w.ws_row, w.ws_col);

    printf("\e[?1049h");

    move_to(0, 0);
    printf("##");
    move_to(1, 2);
    printf("##");
    move_to(2, 4);
    printf("##");

    int a;
    getchar();

    // something
    //printf("one and counting\n");
    //char input = input_timeout(STDIN_FILENO, 1);

    printf("\e[?1049l");

    return 0;
}
