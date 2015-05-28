#ifndef BIG_MANDEL_BMP_H_
#define BIG_MANDEL_BMP_H_

#include "mandelbrot.h"

typedef void (*mandelbrot_calculator) (mandelbrot_data* data, const mandelbrot_info* info);
typedef int (*color_function) (double value);


void stream_mandelbrot_to_bmp(const char* filename, int w, int h, int max_iter,
        mandelbrot_info* info, mandelbrot_calculator calculator, color_function cf);



#endif // BIG_MANDEL_BMP_H_
