#include "../tokenizer.h"
#include "../utils.h"
#include "../custom_table.h"
#include "../TableMap.h"



static bool run_test_tokenizer(char* input, int expected_count, char** expected_tokens) {
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


static void insert_tables(TableMap* T_Map) {
    //Table 1
    const int datatypes1[] = {INT, DOUBLE, VARCHAR};
    const char* column_names1[] = {"ID", "Value", "Name"};
    int column_count1 = 3;
    Table* table1 = create_table(datatypes1, column_names1, column_count1, "Employees");
    insertTable(T_Map, table1);

    //Table 2
    const int datatypes2[] = {VARCHAR, INT, DOUBLE, DOUBLE};
    const char* column_names2[] = {"Name", "ID", "Price", "Discount"};
    int column_count2 = 4;
    Table* table2 = create_table(datatypes2, column_names2, column_count2, "Products");
    insertTable(T_Map, table2);

    //Table 3
    const int datatypes3[] = {DOUBLE, VARCHAR, INT};
    const char* column_names3[] = {"Score", "Subject", "StudentID"};
    int column_count3 = 3;
    Table* table3 = create_table(datatypes3, column_names3, column_count3, "Grades");
    insertTable(T_Map, table3);

    //Table 4
    const int datatypes4[] = {INT, VARCHAR, DOUBLE};
    const char* column_names4[] = {"OrderID", "CustomerName", "TotalAmount"};
    int column_count4 = 3;
    Table* table4 = create_table(datatypes4, column_names4, column_count4, "Orders");
    insertTable(T_Map, table4);

}

static bool find_compare_table_from_tableMap(TableMap* T_Map, char* tableName,
                                             char** expected_columns, const int expected_column_count) {
    Table* desired_table = findTable(T_Map, tableName);
    if (desired_table == NULL) {
        printf("Table '%s' not found in the TableMap\n", tableName);
        return false;
    }

    if ((int)desired_table->column_count != expected_column_count) {
        printf("Error: The column count '%d' doesn't match with expected value '%d'\n", (int)desired_table->column_count, expected_column_count);
        return false;
    }

    for (int i = 0; i < expected_column_count; ++i) {
        if (strcmp(desired_table->columns[i].column_name, expected_columns[i]) != 0) {
            printf("Error: Column '%s' at index %d doesn't match with expected value '%s'\n",
                   desired_table->columns[i].column_name, i, expected_columns[i]);
            return false;
        }
    }

    printf("Success: Table '%s' found with the correct column count and names\n", desired_table->tableName);
    return true;
}

static void run_test_table_creation() {
    TableMap* T_Map = init_TableMap(100);
    insert_tables(T_Map);

    find_compare_table_from_tableMap(T_Map, "Employees", (char*[]) {"ID", "Value", "Name"}, 3);
    find_compare_table_from_tableMap(T_Map, "Products", (char*[]) {"Name", "ID", "Price", "Discount"}, 4);
    find_compare_table_from_tableMap(T_Map, "Grades", (char*[]) {"Score", "Subject", "StudentID"}, 3);
    find_compare_table_from_tableMap(T_Map, "Orders", (char*[]) {"OrderID", "CustomerName", "TotalAmount"}, 3);

    FreeTableMap(T_Map);
}


void serializing_tests(){
    TableMap* T_Map = init_TableMap(100);
    insert_tables(T_Map);

    Row* new_record_1 = InitRecord(T_Map, "Employees", (void*[]){&(int){1}, &(double){5000.0}, strdup("John")});
    Row* new_record_2 = InitRecord(T_Map, "Products", (void*[]){strdup("Shoes"), &(int){1}, &(double){39.99}, &(double){5.40}});
    void* page_1 = serialize_row(new_record_1, T_Map, "Employees");
    void* page_2 = serialize_row(new_record_2, T_Map, "Products");
    Row* recovered_record_1 = deserialize_row(T_Map, page_1);
    Row* recovered_record_2 = deserialize_row(T_Map, page_2);
    print_row(recovered_record_1);
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

    else if(strcmp("table_creation", argv[1]) == 0){
        run_test_table_creation();
    }
    else if(strcmp("serialize", argv[1]) == 0){
        serializing_tests();
    }
    else{
        printf("[HINT]: \n'tokenizer_test' - 1 test\n'table_creation' - 2 test\n'serialize' - 3 test\n");
    }
    return 0;
}
