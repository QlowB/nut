#include <stdio.h>
#include <curses.h>

#include "mandelbrot.h"


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
        compute_data(&md, &mi);
        print_data(&md);
        while(1) {
            int val = getch();
            printw("%d\n", val);
        }
        break;
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
