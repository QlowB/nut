#include "escape_sequences.h"
#include <curses.h>


esc_input csi(int* val);


esc_input esc_read(void)
{
    int val[32];
    val[0] = getch();

    if (val[0] == 033 || val[0] == '^') {
        val[1] = getch();
        if (val[1] == '[') {
            return csi(val);
        }
    } else {
        esc_input ei = { NORMAL, val[0] };
        return ei;
    }
    esc_input ei;
    ei.type = NOTHING;
    return ei;
}



esc_input csi(int* val)
{
    val[3] = getch();

    esc_input ei;
    switch(val[3]) {
        case 'A':
            ei.type = ARROW_UP;
            break;
        case 'B':
            ei.type = ARROW_DOWN;
            break;
        case 'C':
            ei.type = ARROW_RIGHT;
            break;
        case 'D':
            ei.type = ARROW_LEFT;
            break;
    }
    return ei;


    if ((val[3] >= '0' && val[3] <= '9') || val[3] == '[' || 1) {
        val[4] = getch();
        for(int  i=0;i<100;i++) printf("YOOOO\n");
        esc_input ei;
        switch(val[4]) {
            case 'A':
                ei.type = ARROW_UP;
                break;
        }
        return ei;
    } else {
        printf("NAAAA: %d\n", val[3]);
        while(1);
    }
}


