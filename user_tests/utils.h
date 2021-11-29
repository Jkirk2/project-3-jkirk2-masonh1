#ifndef UTILS_H
#define UTILS_H

#include "buffer.h"

extern ring_buffer_421_t* buffer_p;

void print_buffer();
void fill_buffer();
void empty_buffer();
void set_string(char* data);

#endif
