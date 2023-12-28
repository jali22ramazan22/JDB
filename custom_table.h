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
uint32_t get_size(Datatype type);

uint32_t* get_offsets(TableColumn* table, size_t column_count);


/* the hashMap method to store the table structures*/


typedef struct Node{
    char* tableName;
    size_t column_count;
    TableColumn* table_ptr;
    struct Node* next;
}TableNode;


void insertTable(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName, 
    const int column_count, TableColumn* table);

int hash_function(const char* tableName , int mod);

TableNode* create_table_map(__ssize_t TableMap_size);

void free_TableMap(TableNode* tableMap, __ssize_t TableMap_size);

TableNode* find_TableMap(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName);




/* The row manipulations according the runtime tables */

typedef struct node {
    void* data;
    Datatype type;
    struct node* next;
} RowNode;
 
typedef struct list {
    int size;
    RowNode* head;
}Row;

Row* create_row();
void add_to_row(Row* row, void* data, Datatype type);
void* remove_from_row(Row* row);
void free_row(Row* row);


void serialize_row(const char* tableName, const void* values, TableNode* TableMap,
     __ssize_t TableMap_size, void* destination);

Row* deserialize_row(const void* source, TableNode* TableMap, __ssize_t TableMap_size);


#endif