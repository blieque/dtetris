#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "functions.h"
#include "input.h"
#include "rendering.h"

int main() {
    // pre-flight checks
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    // move to alternative screen
    printf("\e[?1049h");

    pthread_t pth_input;
    pthread_t pth_rendering;
    pthread_create(&pth_input, NULL, init_input, NULL);
    pthread_create(&pth_rendering, NULL, init_rendering, NULL);

    pthread_join(pth_input, NULL);
    pthread_cancel(pth_rendering);

    // move out of alternative screen
    printf("\e[?1049l");

    return 0;
}
