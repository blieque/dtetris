#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD

#include <stdlib.h>

char* alloc_board(char*** board, int width, int height);
void free_board(char*** board, char* _board_data);

#endif
