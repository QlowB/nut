#ifndef MANDELBROT_H_
#define MANDELBROT_H_


typedef double real_number;


typedef struct {
    real_number x;
    real_number y;
    real_number width;
    real_number height;
} mandelbrot_info;


typedef struct {
    int img_width;
    int img_height;

    int max_iterations;
    int* iterations;
} mandelbrot_data;


void compute_data(mandelbrot_data* data, const mandelbrot_info* info);

void init_data(int width, int height, int max_iterations,
               mandelbrot_data* data);
void destroy_data(mandelbrot_data* data);


#endif // MANDELBROT_H_

