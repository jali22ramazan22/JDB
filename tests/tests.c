#include "../tokenizer.h"
#include "../utils.h"
#include "../custom_table.h"

bool run_test_tokenizer(char* input, int expected_count, char** expected_tokens) {
    Input_Buffer* input_buffer = new_input_buffer();
    input_buffer->buffer = input;
    input_buffer->buffer_length = strlen(input);
    char** result = tokenize_string(input_buffer);
    for (int i = 0; i < expected_count; ++i) {
        if (strcmp(result[i], expected_tokens[i]) != 0) {
            return false;
        }
    }

    return true;
}


bool run_test_table_creation(){
    int t1_datatypes[] = {INT, INT, DOUBLE};
    char* t1_column_names[] = {"ID", "BALANCE", "HP"};
    TableColumn* new_table = create_table(t1_datatypes, t1_column_names, 3, "player_table");

    

}

int main(int argc, char** argv) {
    if(argc != 2){
        printf("[HINT] try the arguments related to the tests names\n");
        printf("[HINT]: \n'tokenizer_test' - 1 test\n'table_creation' - 2 test\n");
        exit(EXIT_FAILURE);
        return -1;
    }

    if(strcmp("tokenizer_test", argv[1]) == 0){
        printf("Test 1 %s\n", run_test_tokenizer("this is test 1", 4, (char*[]){"this", "is", "test", "1"}) ? "passed" : "failed");
        printf("Test 2 %s\n", run_test_tokenizer("", 0, NULL) ? "passed" : "failed");
        printf("Test 3 %s\n", run_test_tokenizer("yet another test 3", 4, (char*[]){"yet", "another", "test", "3"}) ? "passed" : "failed");
        printf("Test 4 %s\n", run_test_tokenizer("a short test", 3, (char*[]){"a", "short", "test"}) ? "passed" : "failed");
        printf("Test 5 %s\n", run_test_tokenizer("one more test case", 4, (char*[]){"one", "more", "test", "case"}) ? "passed" : "failed");
    }
    else{
        printf("[HINT]: \n'tokenizer_test' - 1 test\n'table_creation' - 2 test\n");
    }
    return 0;
}
