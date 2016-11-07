#include "functions.h"
#include <stdlib.h>

char* alloc_board(char*** board, int width, int height) {
    *board = (char**) malloc(width * sizeof(char*));
    char* board_data = malloc(width * height * sizeof(char));

    for (int i = 0; i < width; i++) {
        (*board)[i] = board_data + i * height;
        for (int j = 0; j < height; j++) {
            (*board)[i][j] = ' ';
        }
    }

    return board_data;
}

void free_board(char*** board, char* _board_data) {
    free(_board_data);
    free(*board);
}
