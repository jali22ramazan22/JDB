#pragma once
#ifndef COMMON_H
#define COMMON_H
#define BUFFER 256
#define MAX_NAME 64
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>
typedef enum{
    START_PROGRAM, //0
    LOOP_PROGRAM, //1
}Prompt_status;


typedef enum{
    META_COMMAND_SUCCESS, //0
    META_COMMAND_UNDEFINED //1
}MetaCommandResult;

typedef enum { 
    PREPARE_SUCCESS, //0
    PREPARE_UNRECOGNIZED_STATEMENT //1
}PrepareResult;

typedef enum { 
    STATEMENT_INSERT, //0
    STATEMENT_SELECT, //1
    STATEMENT_CREATE  //2
} StatementType;

typedef struct{
    StatementType type;
}Statement;
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

typedef struct Container{
    Table* table_ptr;
}TableContainer;

typedef struct{
    TableContainer* hash_table;
    size_t TableMap_Size;
}TableMap;


#endif