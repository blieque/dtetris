#include "game.h"
#include "../types.h"

// Setting `_POSIX_C_SOURCE' didn't work.
#define __USE_POSIX199309
    #include <time.h>
#undef __USE_POSIX199309

static GameData* gd;

/*
 * Initialise the input thread.
 */
void* init_game(void* gd_v) {
    gd = (GameData*) gd_v;

    // Create a delay to use in the game loop.
    struct timespec tick_delay;
    tick_delay.tv_sec = 1;
    tick_delay.tv_nsec = 0;

    // Main game loop.
    while (gd->keep_running) {
        pthread_mutex_lock(&gd->mutex);
        if (gd->cursor.y > 0) gd->cursor.y--;
        pthread_mutex_unlock(&gd->mutex);
        nanosleep(&tick_delay, NULL);
    }

    return NULL;
}
