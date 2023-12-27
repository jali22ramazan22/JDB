#include "custom_table.h"

/* This module trying to implement the creation in the RAM the custom table frame

    The tokenizer + parser goes here and creates the table.

    To do: collect it into the abstract the data part with offset info and store it as a 'page' in RAM


*/

/* The table structure creation */

static void init_column(TableColumn* column, const Datatype datatype, const char* column_name) {
    strcpy(column->column_name, column_name);
    column->type = datatype;
}


TableColumn* create_table(const int* datatypes, const char** column_names, const int column_count, const char* tablename){
    if(datatypes == NULL || column_names == NULL)
        return NULL;
    TableColumn* new_table = (TableColumn*)malloc(sizeof(TableColumn) * (column_count + 2));
    if (new_table == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    init_column(&new_table[0], INT, "ID");
    for(int i = 0; i < column_count + 1; ++i){
        init_column(&new_table[i + 1], datatypes[i], column_names[i]);
    }
    return new_table;
}

void free_table(TableColumn* table){
    free(table);
}

uint32_t get_offset(Datatype type){
    switch (type){
        case INT:
            return 4;
        case VARCHAR:
            return 50;
        case DOUBLE:
            return 8;
        default:
            return 0;
    }
}


/* The row serialization/deserialization*/






/* The table structure storing implementation through the hashMap*/
int hash_function(const char* tableName, int mod) {
    int hash = 0;
    for (int i = 0; i < strlen(tableName); ++i) {
        hash = (hash * 31) + tableName[i];
    }
    return hash % mod;
}

TableNode* create_table_map(__ssize_t TableMap_size) {
    TableNode* tableMap = (TableNode*)malloc(sizeof(TableNode) * TableMap_size);
    if (tableMap == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TableMap_size; ++i) {
        tableMap[i].tableName = NULL;
        tableMap[i].table_ptr = NULL;
        tableMap[i].next = NULL;
    }
    return tableMap;
}

void insertTable(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName, TableColumn* table) {
    if (tableMap == NULL || TableMap_size <= 0 || tableName == NULL || table == NULL) {
        return;
    }
    int tableHashCode = hash_function(tableName, TableMap_size);
    TableNode* iterator = &tableMap[tableHashCode];
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    iterator->next = (TableNode*)malloc(sizeof(TableNode));
    if (iterator->next == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    iterator->next->table_ptr = table;
    iterator->next->next = NULL;
    iterator->next->tableName = strdup(tableName);
    if (iterator->next->tableName == NULL) {
        perror("Failed to allocate memory for tableName");
        exit(EXIT_FAILURE);
    }
}

TableNode* find_TableMap(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName) {
    int hash = hash_function(tableName, TableMap_size);
    TableNode* iterator = &tableMap[hash];
    while (iterator != NULL) {
        if (strcmp(iterator->tableName, tableName) == 0) {
            return iterator;
        }
        iterator = iterator->next;
    }
    return NULL;  
}

void free_TableMap(TableNode* tableMap, __ssize_t TableMap_size) {
    for (int i = 0; i < TableMap_size; ++i) {
        TableNode* iterator = &tableMap[i];
        while (iterator->next != NULL) {
            TableNode* temp = iterator->next;
            iterator->next = temp->next; 
            free(temp->tableName);  
            free(temp);  
        }
    }
    free(tableMap);
}