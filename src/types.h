#ifndef INCLUDED_TYPES
#define INCLUDED_TYPES

#include <stdint.h>
#include <pthread.h>

#include "board.h"

typedef enum {
    false = 0,
    true = 1
} bool;

typedef struct {
    uint16_t x;
    uint16_t y;
} Point;

typedef struct {
    pthread_mutex_t mutex;

    bool keep_running;
    float frame_rate;
    float frame_interval;

    uint16_t width;
    uint16_t height;
    Point cursor;
    Board board;
} GameData;

#endif
