#pragma once
#ifndef UTILS_H
#include "common.h"

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} Input_Buffer;

Input_Buffer* new_input_buffer(void);
void close_input_buffer(Input_Buffer*);
void read_input(Input_Buffer*);

void autocomplete_input(Input_Buffer*);

struct tm* print_time(void);

#endif 