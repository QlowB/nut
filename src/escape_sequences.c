#include "escape_sequences.h"
#include <curses.h>

esc_input esc_read(void)
{
    int val[32];
    val[0] = getch();

    if (val[0] = 033) {

    } else {
        esc_input ei = { NORMAL, val[0] };
        return ei;
    }
}


