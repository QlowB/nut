#include "big_mandel_bmp.h"
#include <stdio.h>
#include <stdlib.h>

void stream_mandelbrot_to_bmp(const char* filename, int w, int h, int max_iter,
        mandelbrot_info* info, mandelbrot_calculator calculator, color_function cf)
{
    unsigned char fileheader[14] = {'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char infoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char padding[3] = {0, 0, 0};

    int filesize = sizeof fileheader + sizeof infoheader;
    filesize += ((4 - (w * 3) % 4) % 4) * h;

    fileheader[ 2] = (unsigned char)(filesize      );  
    fileheader[ 3] = (unsigned char)(filesize >>  8); 
    fileheader[ 4] = (unsigned char)(filesize >> 16);
    fileheader[ 5] = (unsigned char)(filesize >> 24);
                
    infoheader[ 4] = (unsigned char)(w      );  
    infoheader[ 5] = (unsigned char)(w >>  8); 
    infoheader[ 6] = (unsigned char)(w >> 16);
    infoheader[ 7] = (unsigned char)(w >> 24);
    infoheader[ 8] = (unsigned char)(h      );  
    infoheader[ 9] = (unsigned char)(h >>  8); 
    infoheader[10] = (unsigned char)(h >> 16);
    infoheader[11] = (unsigned char)(h >> 24);


    FILE* file = fopen(filename, "wb");
    fwrite(fileheader, 1, sizeof fileheader, file);
    fwrite(infoheader, 1, sizeof infoheader, file);

    int written_height = 0;
    int pixels = 1024 * 1024;

    for(written_height = 0; written_height < h;) {

        int max_y = written_height + 1 + (pixels / w);
        if (max_y > h)
            max_y = h;

        mandelbrot_data data;
        mandelbrot_info ninfo;
        init_data(w, max_y - written_height, max_iter, &data);
        ninfo.x = info->x;
        ninfo.width = info->width;
        ninfo.y = info->y + info->height * written_height / h;
        ninfo.height = info->height * (max_y - written_height) / h;

        calculator(&data, &ninfo);

        char* line = malloc(w * 3);

        for (int j = written_height; j < max_y; j++) {
            for (int k = 0; k < w; k++) {
                int color = cf((double) data.iterations[w * (j - written_height) + k] / max_iter);
                line[k * 3 + 0] = color & 0xFF; 
                line[k * 3 + 1] = (color >> 8) & 0xFF; 
                line[k * 3 + 2] = (color >> 16) & 0xFF; 
            } 
            fwrite(line, 3, w, file);
            fwrite(padding, 1, ((4 - (w * 3) % 4) % 4), file);
        }
        free(line);

        destroy_data(&data);
        written_height = max_y;
    }
    fclose(file);
}


