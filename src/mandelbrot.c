#include "mandelbrot.h"
#include <stdlib.h>
#include <string.h>

int iterate(real_number x, real_number y, int max_iterations) {

    real_number a = x;
    real_number b = y;

    for (int i = 0; i < max_iterations; i++) {
        real_number aa = a * a;
        real_number bb = b * b;
        real_number ab = a * b;
        a = aa - bb + x;
        b = ab + ab + y;

        if (aa + bb > 16.0f) {
            return i;
        }
    }
    return max_iterations;
}


void compute_data(mandelbrot_data* data, const mandelbrot_info* info)
{
    int iw = data->img_width;
    int ih = data->img_height;
    int max_iter = data->max_iterations;
#pragma omp parallel
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            int* data_ptr = &data->iterations[i * iw + j];
            *data_ptr = iterate(
                        info->x + j * info->width / iw,
                        info->y + i * info->height / ih,
                        max_iter
            );
        }
    }
}


void init_data(int width, int height, int max_iterations,
               mandelbrot_data* data)
{
    data->img_width = width;
    data->img_height = height;
    data->max_iterations = max_iterations;
    data->iterations = malloc(width * height * sizeof(*data->iterations));
}


void destroy_data(mandelbrot_data* data)
{
    if (data->iterations != 0)
        free(data->iterations);
    memset(&data, 0, sizeof(data));
}

