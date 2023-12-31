#include "tokenizer.h"
#include "common.h"
#include "utils.h"
#include "custom_table.h"
static __ssize_t TableMap_size = 5;





void print_prompt(int status){
    if(status == START_PROGRAM){
        struct tm* tm = print_time();
        printf("JDB. version 0.1. Clone of SQLite3 >> %s", asctime(tm));
        printf("HINT: use the '.help' to see the all commands\n");
        printf("The support on hard-coded table only with columns: id, username, email\n");
        printf("Done by Jalil Ramazan and inspired by https://cstack.github.io/db_tutorial/ \n");
        return;
    }
    printf("db > ");
}

void print_hint(void){
    printf(".exit - stop the program\n.help - show all commands\n");
}

MetaCommandResult do_meta_command(Input_Buffer* input_buffer){
    if(strcmp(input_buffer->buffer, ".exit") == 0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(input_buffer->buffer, ".help") == 0){
        print_hint();
        return META_COMMAND_SUCCESS;
    }
    else{
        return META_COMMAND_UNDEFINED;
    }
}

//just tokenizing the string
void temporary(Input_Buffer* input_buffer){
    read_input(input_buffer);
    char** tokenized_buffer = tokenize_string(input_buffer);
    for(int i = 0; i < count_words(input_buffer->buffer, input_buffer->buffer_length); ++i){
        printf("%s\n", tokenized_buffer[i]);
    }
}


PrepareResult prepare_statement(Input_Buffer* input_buffer, Statement* statement) {
  if(strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if(strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  if(strcmp(input_buffer->buffer, "create") == 0){
    statement->type = STATEMENT_CREATE;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            printf("This is where we would do an insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("This is where we would do a select.\n");
            break;
        case (STATEMENT_CREATE):
            printf("This is where we would do a create.\n");
            break;
    }
}



int main(int argc, char** argv){
    Input_Buffer* input_buffer = new_input_buffer();
    print_prompt(START_PROGRAM);

    while(true){
        print_prompt(LOOP_PROGRAM);

        read_input(input_buffer);
        if(input_buffer->buffer[0] == '.'){
            switch(do_meta_command(input_buffer)){
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNDEFINED:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)){
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
            execute_statement(&statement);
            printf("Executed.\n");
        }
    }
}
