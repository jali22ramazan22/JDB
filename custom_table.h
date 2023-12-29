#pragma once
#ifndef CUSTOM_TABLE_H
#define CUSTOM_TABLE_H
#include "common.h"
//TODO: write the serialization/deserialization according new hashing func
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

typedef struct{
    TableColumn* columns;
    size_t column_count;
    char tableName[MAX_NAME];
}Table;

Table* create_table(const int *Datatypes, const char **ColumnNames, const int ColumnCount, const char* TableName);
void free_table(Table* table);

uint32_t get_size(Datatype type);
uint32_t* get_offsets(Table* table);



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





#endif