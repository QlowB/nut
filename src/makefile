# 
# very basic makefile
#
CFLAGS := -std=c99 -O2
LINKFLAGS := -framework OpenCl -lncurses
OBJECTS := mandelbrot.o main.o big_mandel_bmp.o

.PHONY: all
all: $(OBJECTS)
	gcc $(OBJECTS) -o nut $(LINKFLAGS)

mandelbrot.o: mandelbrot.c
	gcc $< -c -o $@ -std=c99 -O3

%.o:%.c
	gcc $< -c -o $@ $(CFLAGS)


