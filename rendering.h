#include <stdio.h>
#include <unistd.h>

void move_to(int x, int y) {
    if (x > 0 && y > 0) {
        x++;
        y++;
        printf("\e[%d;%dH", x, y);
    }
}

void *init_rendering(void *gd) {
    while (1) {
        printf("##\e[2D");
        sleep(1);
    }
}
