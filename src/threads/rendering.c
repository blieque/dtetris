#include "rendering.h"
#include "../types.h"
#include "../board.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Setting `_POSIX_C_SOURCE' didn't work.
#define __USE_POSIX199309
    #include <time.h>
#undef __USE_POSIX199309

static GameData* gd;
static Board last_board;

/*
 * 8-bit colour codes for our chosen scheme. Background ("_bg") colour codes are
 * used for terminal cell backgrounds, and forground ("_fg") colours for text.
 */
static const uint8_t colors_bg[] = {
    0, 18, 20, 25, 27, 32, 39, 44, 49
};
/*
static const uint8_t colors_fg[] = {
    0, 49, 18, 20, 25, 27, 32, 39, 44
};
*/

/*
 * Prints an escape sequence telling the terminal to move the cursor to the
 * provided co-ordinates. `x' values are doubled because game cells are two
 * terminal cells wide. `y' values are subtracted from the height of the board.
 * This makes the bottom left the origin, and higher `y' values correspond to
 * cells higher up screen.
 */
static void move_to(int16_t x, int16_t y) {
    // Check that we aren't overshooting the bounds of the board.
    if (x >= 0 && y >= 0 && x < gd->width && y < gd->height) {
        x *= 2;
        y = gd->height - y - 1;
        // Terminal rows and columns are not zero-indexed, so both co-ordinates
        // are incremented.
        x++; y++;
        printf("\033[%d;%dH", y, x);
    }
}

/*
 * Draws the cell at ths given co-ordinates.
 */
static void draw_cell(uint8_t x, uint8_t y) {
    move_to(x, y);

    uint8_t cell_value = gd->board.cell[x][y];
    if (cell_value == 0) {
        printf("\033[0m  ");
    } else {
        printf("\033[48;5;%dm  ", colors_bg[cell_value]);
    }
}

/*
 * Calculates the difference between the desired game board state and the state
 * showing in the terminal. This allows us to minimise the use of escape
 * sequences when no re-drawing is required.
 */
static void render() {
    // Iterate over the board cells and render those which have changed.
    for (int i = 0; i < gd->width; i++) {
        for (int j = 0; j < gd->height; j++) {
            // Check for change in board state.
            if (gd->board.cell[i][j] != last_board.cell[i][j]) draw_cell(i, j);
        }
    }
}

/*
 * Initialise the rendering thread.
 */
void* init_rendering(void* gd_p) {
    gd = (GameData*) gd_p;
    // Create a second game board to enable diff-based rendering.
    last_board = Board_new(gd->width, gd->height);

    // Create a delay to use in the render loop.
    struct timespec frame_delay;
    frame_delay.tv_sec = 0;
    frame_delay.tv_nsec = 16 * 1000000;

    // Main render loop.
    while (gd->keep_running) {
        // Add the cursor to the board for rendering, then remove it after. The
        // board is copied before the cursor is removed so that the renderer
        // removes the cursor next frame.
        gd->board.cell[gd->cursor.x][gd->cursor.y] = 1;
        render();
        Board_copyData(&last_board, &gd->board);
        gd->board.cell[gd->cursor.x][gd->cursor.y] = 0;
        move_to(0, 0);

        nanosleep(&frame_delay, NULL);
    }

    // Free the second board before closing the thread.
    Board_free(&last_board);

    return NULL;
}
