#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <ncurses.h>

#ifdef __APPLE__
#include <OpenCl/OpenCl.h>
#else
#include <CL/cl.h>
#endif

#include "mandelbrot.h"
#include "escape_sequences.h"


int process_input(mandelbrot_info* info, mandelbrot_data* data);
int process_input_string(const char* str, mandelbrot_info* info, mandelbrot_data* data);


void print_data(mandelbrot_data* data);
char* read_file(const char* filename, size_t* length);
void write_bmp(const char* filename, mandelbrot_data* data);


WINDOW* main_wnd;

mandelbrot_info mi;
mandelbrot_data md;

int main(int argc, char** argv)
{
    main_wnd = initscr();

    keypad(main_wnd, TRUE);

    mi.x = -2;
    mi.y = -2;
    mi.width = 4;
    mi.height = 4;

    init_data(60, 30, 200, &md);

    md.img_width = getmaxx(main_wnd) - 1;
    md.img_height = getmaxy(main_wnd) - 1;
    mi.height = mi.width * md.img_height / md.img_width * 2.0;
    clear();
    compute_data(&md, &mi);
    print_data(&md);

    while (1) {
        int resp = process_input(&mi, &md);
        if (resp == 2) {
        	char input[2048];
        	getstr(input);
        	process_input_string(input, &mi, &md);
        }
    }


    destroy_data(&md);


    endwin();
}


int process_input(mandelbrot_info* info, mandelbrot_data* data)
{
    int need_recalc = 1;
    int ch = getch();

    real_number x_grow = 0.0;
    real_number y_grow = 0.0;
    
    switch (ch) {
    case KEY_UP:
        info->y -= info->height / data->img_height;
        break;
    case KEY_DOWN:
        info->y += info->height / data->img_height;
        break;
    case KEY_LEFT:
        info->x -= 2 * info->width / data->img_width;
        break;
    case KEY_RIGHT:
        info->x += 2 * info->width / data->img_width;
        break;
    case 10: // Enter
    	return 2;
    	break;
    case KEY_RESIZE:
    	break;
    case '+':
        x_grow = -info->width * 0.1;
        y_grow = -info->height * 0.1;
        break;
    case '-':
        x_grow = info->width * 0.1;
        y_grow = info->height * 0.1;
        break;
    case '1':
        data->max_iterations += 10;
        break;
    case '2':
        data->max_iterations -= 10;
        break;
    default:
    	return 0;
    }
    
    
    info->width += x_grow;
    info->height += y_grow;
    info->x -= x_grow / 2;
    info->y -= y_grow / 2;

    data->img_width = getmaxx(main_wnd) - 1;
    data->img_height = getmaxy(main_wnd) - 1;
    if (need_recalc) {
        compute_data(data, info);
        info->height = info->width * data->img_height / data->img_width * 2.0;
        clear();
        print_data(&md);
    }

    return 1;
}


int process_input_string(const char* str, mandelbrot_info* info, mandelbrot_data* data)
{
	if (strcmp(str, "exit") == 0) {
		endwin();
		exit(0);
	}
	else if (memcmp(str, "export ", 7) == 0) {
		int w, h, max_iter;
		sscanf(str + 6, "%d %d %d", &w, &h, &max_iter);
		mandelbrot_data mega_data;
		init_data(w, h, max_iter, &mega_data);
		compute_data(&mega_data, info);
		write_bmp("out.bmp", &mega_data);
		destroy_data(&mega_data);
	}
	return 0;
}


void print_data(mandelbrot_data* data)
{
    int iw = data->img_width;
    int ih = data->img_height;

    int max_color = 0;

    start_color();

    init_pair(++max_color, COLOR_BLACK, COLOR_RED);
    init_pair(++max_color, COLOR_BLACK, COLOR_GREEN);
    init_pair(++max_color, COLOR_BLACK, COLOR_YELLOW);
    init_pair(++max_color, COLOR_BLACK, COLOR_BLUE);
    init_pair(++max_color, COLOR_BLACK, COLOR_CYAN);
    init_pair(++max_color, COLOR_BLACK, COLOR_BLACK);

    int cp = 0;
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            int dat = data->iterations[i * iw + j];
            float val = ((float) dat) / data->max_iterations;
            int new_cp = 1 + (char) (pow(val, 0.15) * (max_color - 1));
            if (cp != new_cp) {
                attroff(COLOR_PAIR(cp));
                attron(COLOR_PAIR(new_cp));
                cp = new_cp;
            }
            printw(" ");
        }
        printw("\n");
    }
    attroff(COLOR_PAIR(cp));
}


char* read_file(const char* filename, size_t* length)
{
    FILE* file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* text = malloc((*length) + 1);
    fread(text, *length, 1, file);
    return text;
}


int color_func(int iterations)
{
	double sinus = sin(0.01 * pow(iterations, 2.5));
	double sinus2 = sin(0.17 * pow(iterations, 0.3));
	return (unsigned char) ((sinus + 1) * 127.5) + 
		   ((unsigned char) ((sinus2 + 1) * 127.5) << 8) +
		   ((unsigned char) ((sinus2 + 1) * 127.5) << 8);
}


// copy from school
void write_bmp(const char* filename, mandelbrot_data* data)
{
    FILE* f;
    unsigned char* img = NULL;
    const int imgsize = 3 * data->img_width * data->img_height;
    const int filesize = 54 + imgsize;
    img = (unsigned char *) malloc(imgsize);
    memset(img, 0, imgsize);
    
    
    
    for(int i = 0; i < data->img_height; i++)
    {
        for(int j = 0; j < data->img_width; j++)
        {
            //int r = to_int(red[i + j * data->img_height]);
            //int g = to_int(green[(i + j * data->img_height)]);
            //int b = to_int(blue[(i + j * data->img_height)]);
            int iter = data->iterations[i * data->img_width + j];
            int color = color_func(iter);
            if (iter == data->max_iterations)
            	color = 0;
            int r = color & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = (color >> 16) & 0xFF;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            img[(j + i * data->img_width) * 3 + 2] = (unsigned char)(r);
            img[(j + i * data->img_width) * 3 + 1] = (unsigned char)(g);
            img[(j + i * data->img_width) * 3 + 0] = (unsigned char)(b);
        }
    }
    
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0, 0, 0};
    
    bmpfileheader[ 2] = (unsigned char)(filesize      );
    bmpfileheader[ 3] = (unsigned char)(filesize >>  8);
    bmpfileheader[ 4] = (unsigned char)(filesize >> 16);
    bmpfileheader[ 5] = (unsigned char)(filesize >> 24);
    
    bmpinfoheader[ 4] = (unsigned char)(data->img_width       );
    bmpinfoheader[ 5] = (unsigned char)(data->img_width  >>  8);
    bmpinfoheader[ 6] = (unsigned char)(data->img_width  >> 16);
    bmpinfoheader[ 7] = (unsigned char)(data->img_width  >> 24);
    bmpinfoheader[ 8] = (unsigned char)(data->img_height      );
    bmpinfoheader[ 9] = (unsigned char)(data->img_height >>  8);
    bmpinfoheader[10] = (unsigned char)(data->img_height >> 16);
    bmpinfoheader[11] = (unsigned char)(data->img_height >> 24);
    
    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    int m;
    for(m = 0; m < data->img_height; m++)
    {
        fwrite(img + (data->img_width * (data->img_height - m - 1) * 3), 3, data->img_width, f);
        fwrite(bmppad, 1, (4 - (data->img_width * 3) % 4) % 4, f);
    }
    fclose(f);
}


