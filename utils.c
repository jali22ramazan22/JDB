#include "utils.h"


Input_Buffer* new_input_buffer(void){
    Input_Buffer* input_buffer = (Input_Buffer*)malloc(sizeof(Input_Buffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
    return input_buffer;
}


void close_input_buffer(Input_Buffer* input_buffer){
    if(input_buffer == NULL){
        printf("Warning: Input_Buffer is empty. Double-free is detected.");
        return;
    }
    free(input_buffer->buffer);
    free(input_buffer);
}



void read_input(Input_Buffer* input_buffer){    
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if(bytes_read <= 0){
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    //skipping '\n'
    input_buffer->buffer_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = '\0';

}

struct tm* print_time(void){
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    return tm;
}