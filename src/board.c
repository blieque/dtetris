#include "board.h"
#include "types.h"

#include <stdlib.h>

/*
 * Requests a chunk of memory from the OS for the game board, and fills it
 * programmatically with a 2D array. This makes freeing memory later easier.
 */
Board Board_new(uint16_t width, uint16_t height) {
    Board board;
    board.width = width;
    board.height = height;

    board.cell = malloc(width * sizeof(void*));
    board.data = malloc(width * height);

    for (uint16_t x = 0; x < width; x++) {
        board.cell[x] = &board.data[x * height];
        for (uint16_t y = 0; y < height; y++) {
            board.cell[x][y] = 0;
        }
    }

    return board;
}

/*
 * Frees the memory allocated for a game board by `Board_new()'.
 */
void Board_free(Board* board) {
    free(board->data);
    free(board->cell);
}

/*
 * Copy the data from one one board struct to another, after checking that their
 * dimensions are the same.
 */
void Board_copyData(Board* b1, Board* b2) {
    if (b1->width == b2->width && b1->height == b2->height) {
        memcpy(b1->data, b2->data, b2->width * b2->height * sizeof(uint8_t));
    }
}

/*
 * Creates a new copy of a board, which allocates new memory and will require
 * freeing after use.
 */
Board Board_duplicate(Board* b1) {
    // Create a new struct with the given board's dimensions.
    Board b2 = Board_new(b1->width, b1->height);
    Board_copyData(&b2, b1);

    return b2;
}
