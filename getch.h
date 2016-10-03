#include <stdio.h>
#include <termios.h>

static struct termios old, new;

/* Set new terminal I/O settings */
void set_termios(void) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new = old; /* make new settings same as old settings */
    new.c_lflag &= ~ICANON; /* disable buffered i/o */
    new.c_lflag &= ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal I/O settings */
void reset_termios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Reade1 character */
char getch(void) {
    char ch;
    set_termios();
    ch = getchar();
    reset_termios();
    return ch;
}
