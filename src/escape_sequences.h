#ifndef ESCAPE_SEQUENCES_H_
#define ESCAPE_SEQUENCES_H_
#include <stdio.h>

typedef enum {
    NORMAL,
    ESCAPE,
    ARROW_UP,
} esc_type;


typedef struct {
    esc_type type;
    int value;
} esc_input;


esc_input esc_read(void);


#endif // ESCAPE_SEQUENCES_H_

