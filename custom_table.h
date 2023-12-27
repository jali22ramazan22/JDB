#pragma once
#ifndef CUSTOM_TABLE_H
#define CUSTOM_TABLE_H
#include "common.h"
/* The prototypes and structs  of table skeleton */
typedef enum{
    INT,
    VARCHAR,
    DOUBLE
} Datatype;


typedef struct {
    Datatype type;
    char column_name[BUFFER];
}TableColumn;

TableColumn* create_table(const int* datatypes, const char** column_names, const int column_count, const char* tablename);
void free_table(TableColumn* table);
uint32_t get_offset(Datatype type);

/* The row manipulations according the runtime tables */



/* the hashMap method to store the table structures*/
typedef struct Node{
    char* tableName;
    TableColumn* table_ptr;
    struct Node* next;
}TableNode;


void insertTable(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName, TableColumn* table);
int hash_function(const char* tableName , int mod);
TableNode* create_table_map(__ssize_t TableMap_size);
void free_TableMap(TableNode* tableMap, __ssize_t TableMap_size);
TableNode* find_TableMap(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName);



#endif