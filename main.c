#include "tokenizer.h"
#include "common.h"
#include "utils.h"
#include "query_generator.h"
#include "custom_table.h"

static size_t TABLE_MAP_SIZE = 100;
void print_logo(){
    printf("\n"
           "   $$$$$\\ $$$$$$$\\  $$$$$$$\\  \n"
           "   \\__$$ |$$  __$$\\ $$  __$$\\ \n"
           "      $$ |$$ |  $$ |$$ |  $$ |\n"
           "      $$ |$$ |  $$ |$$$$$$$\\ |\n"
           "$$\\   $$ |$$ |  $$ |$$  __$$\\ \n"
           "$$ |  $$ |$$ |  $$ |$$ |  $$ |\n"
           "\\$$$$$$  |$$$$$$$  |$$$$$$$  |\n"
           " \\______/ \\_______/ \\_______/ \n"
           "                              \n"
           "                              \n");
}

void print_prompt(int status){
    if(status == START_PROGRAM){
        print_logo();
        struct tm* tm = print_time();
        printf("JDB. version 0.1. Clone of SQLite3 >> %s", asctime(tm));
        printf("HINT: use the '.help' to see the all commands\n");
        printf("There's the support with dynamic tables with custom columns with types (INTEGER, VARCHAR, DOUBLE)\n");
        printf("Due to the lack of parser you should guarantee that your input is correct, otherwise the behavior of program is undefined\n\n");
        printf("Done by Jalil Ramazan and inspired by https://cstack.github.io/db_tutorial/ \n");
        printf("// \"create table <NAME> with columns <NAME> <DATATYPE>, ...\"\n"
               "// \"insert in table <NAME> values <VALUE_1>, <VALUE_2> ...\"\n"
               "// \"select <COLUMN 1> <COLUMN 2> ...  from table <NAME> where <EXPRESSION> \"\n");
        return;
    }
    printf("db > ");
}

void print_hint(void){
    printf(".exit - stop the program\n.help - show all commands\n.all_tables - print all tables\n");
}

MetaCommandResult do_meta_command(Input_Buffer* input_buffer, TableMap* T_Map){
    if(strcmp(input_buffer->buffer, ".exit") == 0){
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(input_buffer->buffer, ".help") == 0){
        print_hint();
        return META_COMMAND_SUCCESS;
    }
    else if(strcmp(input_buffer->buffer, ".all_tables") == 0){
        PrintAllTables(T_Map);
        return META_COMMAND_SUCCESS;
    }
    else{
        return META_COMMAND_UNDEFINED;
    }
}


// "create table <NAME> with columns <NAME> <DATATYPE>, ..."
// "insert in table <NAME> values <VALUE_1>, <VALUE_2> ..."
// "select <COLUMN 1> <COLUMN 2> ...  from table <NAME> where <EXPRESSION> "


PrepareResult prepare_statement(char** tokenized_input, Statement* statement) {
  if(strcmp(tokenized_input[0], "insert") == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }
  if(strcmp(tokenized_input[0], "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }
  if(strcmp(tokenized_input[0], "create") == 0){
    statement->type = STATEMENT_CREATE;
    return PREPARE_SUCCESS;
  }
  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement* statement, char** tokenized_input, size_t tokens_count, TableMap* T_Map) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            QueryInsert(tokenized_input, tokens_count, T_Map);
            break;
        case (STATEMENT_SELECT):
            QuerySelect(tokenized_input, tokens_count, T_Map);
            break;
        case (STATEMENT_CREATE):
            QueryCreate(tokenized_input, tokens_count, T_Map);
            break;
    }
}



int main(int argc, char** argv){
    Input_Buffer* input_buffer = new_input_buffer();
    TableMap* T_Map = init_TableMap(TABLE_MAP_SIZE);
    print_prompt(START_PROGRAM);

    while(true){
        print_prompt(LOOP_PROGRAM);
        read_input(input_buffer);
        if(input_buffer->buffer[0] == '.'){
            switch(do_meta_command(input_buffer, T_Map)){
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNDEFINED:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }
        else{
            size_t tokens_count = 0;
            char** tokenized_input = tokenize_string(input_buffer, &tokens_count);
            Statement statement;
            switch (prepare_statement(tokenized_input, &statement)){
                case (PREPARE_SUCCESS):
                    execute_statement(&statement, tokenized_input, tokens_count, T_Map);
                    printf("Executed.\n");
                    break;
                case (PREPARE_UNRECOGNIZED_STATEMENT):
                    printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                    continue;

            }
        }

    }
    FreeTableMap(T_Map);
    close_input_buffer(input_buffer);
}
