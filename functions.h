int round_float(float x) {
    return x > 0 ? (int)(x + 0.5) : (int)(x - 0.5);
}

void alloc_board(char** board, int width, int height) {
    board = (char**) malloc(width * sizeof(char*));
    for (int i = 0; i < width; i++) {
        board[i] = (char*) malloc(height * sizeof(char));
    }
}

void free_board(char*** board) {
    int columns = sizeof(*board) / sizeof(*board[0]);
    for (int i = 0; i < columns; i++) {
        free(*board[i]);
    }
    free(*board);
}
