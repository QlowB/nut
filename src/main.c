#include <stdio.h>
#include <curses.h>

#include "mandelbrot.h"
#include "escape_sequences.h"

void print_data(mandelbrot_data* data);

int main(int argc, char** argv)
{
    WINDOW* main_wnd;
    main_wnd = initscr();


    mandelbrot_info mi;
    mandelbrot_data md;


    mi.x = -2;
    mi.y = -2;
    mi.width = 4;
    mi.height = 4;

    init_data(60, 30, 200, &md);


    while (1) {
        clear();
        
        md.img_width = getmaxx(main_wnd) - 1;
        md.img_height = getmaxy(main_wnd) - 1;
        
        mi.height = mi.width * md.img_height / md.img_width * 2.0;

        printf("%f\n", (float) mi.height);

        compute_data(&md, &mi);
        print_data(&md);
        
        esc_input ei;
        ei = esc_read();
        switch (ei.type) {
            case ARROW_UP:
            mi.y -= mi.height / md.img_height;
            break;
            case ARROW_DOWN:
            mi.y += mi.height / md.img_height;
            break;
            case ARROW_RIGHT:
            mi.x += mi.width / md.img_width;
            break;
            case ARROW_LEFT:
            mi.x -= mi.width / md.img_width;
            break;
            case NORMAL:
            {
                real_number x_grow = 0.0;
                real_number y_grow = 0.0;
                switch (ei.value) {
                    case '+':
                        x_grow = -mi.width * 0.1;
                        y_grow = -mi.height * 0.1;
                        break;
                    case '-':
                        x_grow = mi.width * 0.1;
                        y_grow = mi.height * 0.1;
                        break;
                }
                mi.width += x_grow;
                mi.height += y_grow;
                mi.x -= x_grow / 2;
                mi.y -= y_grow / 2;
            }
            default: break;
        }
    }


    destroy_data(&md);


    endwin();
}



void print_data(mandelbrot_data* data)
{
    int iw = data->img_width;
    int ih = data->img_height;
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            int dat = data->iterations[i * iw + j];
            float val = ((float) dat) / data->max_iterations;
            char letter;
            if (dat == data->max_iterations)
                letter = ' ';
            else
                letter= 'A' + (char) (val * 26);
            printw("%c", letter);
        }
        printw("\n");
    }
}
