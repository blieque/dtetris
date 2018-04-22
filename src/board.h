#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD

#include <stdlib.h>

typedef struct {
    pthread_mutex_t mutex;

    uint16_t width;
    uint16_t height;
    uint8_t** cell;
    uint8_t* data;
} Board;

Board Board_new(uint16_t width, uint16_t height);
void Board_free(Board* board);

void Board_copyData(Board* b1, Board* b2);
Board Board_copy(Board* board);

#endif
