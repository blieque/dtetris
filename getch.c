#include "getch.h"
#include <stdio.h>
#include <termios.h>

static struct termios old;
static struct termios new;

/* Set new terminal I/O settings */
static void set_termios(void) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal I/O settings */
static void reset_termios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character */
char getch(void) {
    set_termios();
    char ch = getchar();
    reset_termios();
    return ch;
}
