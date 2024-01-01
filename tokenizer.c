#include "tokenizer.h"



size_t count_words(char* str, int len) {
    if (str == NULL)
        return 0;

    size_t words = 0;
    int in_word = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] != ' ' && !in_word) {
            in_word = 1;
            ++words;
        }
        else if (str[i] == ' ') {
            in_word = 0;
        }
    }
    return words;
}

int* words_len(char* str, int len) {
    if (str == NULL)
        return NULL;
    size_t words_count = count_words(str, len);
    int* words_arr = (int*)malloc(sizeof(int) * words_count);
    if (words_arr == NULL) {

        return NULL;
    }

    int j = 0;
    int word_length = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] != ' ') {
            ++word_length;
        }
        else if (word_length > 0) {
            words_arr[j++] = word_length;
            word_length = 0;
        }
    }


    if (word_length > 0) {
        words_arr[j] = word_length;
    }

    return words_arr;
}





char** tokenize_string(Input_Buffer* input_buffer, size_t* tokens_count) {
    char** words_array = NULL;
    int* words_len_array = NULL;

    if (input_buffer == NULL) {
        return words_array;
    }

    size_t words_count = count_words(input_buffer->buffer, (int)input_buffer->buffer_length);

    if (words_count > 0) {
        words_array = (char**)malloc(sizeof(char*) * words_count);
        words_len_array = words_len(input_buffer->buffer, (int)input_buffer->buffer_length);

        if (words_array == NULL || words_len_array == NULL) {
            printf("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        int offset = 0;
        for (int i = 0; i < words_count; ++i) {
            words_array[i] = (char*)malloc(sizeof(char) * (words_len_array[i] + 1));

            if (words_array[i] == NULL) {
                printf("Memory allocation error");
                exit(EXIT_FAILURE);
            }

            strncpy(words_array[i], input_buffer->buffer + offset, words_len_array[i]);
            words_array[i][words_len_array[i]] = '\0';
            offset += words_len_array[i] + 1;


            to_lower_string(words_array[i]);
        }

        free(words_len_array);
    }

    *tokens_count = words_count;
    return words_array;
}

