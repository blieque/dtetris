#include "getch.h"
#include <stdio.h>
#include <termios.h>

static struct termios current;
static struct termios new;

/*
 * Set new terminal I/O settings.
 */
static void set_termios(void) {
    // Back up current terminal I/O settings to restore to later.
    tcgetattr(0, &current);
    // Copy current settings for modification.
    new = current;
    // Disable buffered I/O and echoing.
    new.c_lflag &= ~ICANON;
    new.c_lflag &= ~ECHO;
    // Activate these new settings.
    tcsetattr(0, TCSANOW, &new);
}

/*
 * Restore current terminal I/O settings.
 */
static void reset_termios(void) {
    tcsetattr(0, TCSANOW, &current);
}

/*
 * Read 1 character from the buffer and return it.
 */
char getch(void) {
    set_termios();
    char ch = getchar();
    reset_termios();
    return ch;
}
