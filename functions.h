#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdlib.h>

char* alloc_board(char*** board, int width, int height);
void free_board(char*** board, char* _board_data);

#endif
