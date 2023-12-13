#pragma once
#ifndef TOKENIZER_H
#include "common.h"
#include "utils.h"


size_t count_words(char* string, int len);
char** tokenize_string(Input_Buffer* input_buffer);
int* words_len(char* str, int len);

#endif