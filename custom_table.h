#pragma once
#ifndef CUSTOM_TABLE_H
#include "common.h"

typedef enum{
    INT,
    VARCHAR,
    DOUBLE
} Datatype;


typedef struct {
    int type;
    char column_name[BUFFER];
}TableColumn;


TableColumn* create_table(int* datatypes, char** column_names, int column_count);
void insert_data(void* data, TableColumn* column);
#endif