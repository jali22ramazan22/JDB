#ifndef JRD_TABLEMAP_H
#define JRD_TABLEMAP_H

#endif //JRD_TABLEMAP_H
#include "custom_table.h"
#include "common.h"

// key - The Table Name
// value - The Table Structure (Pointer on the structure)



typedef struct Container{
    Table* table_ptr;
}TableContainer;

typedef struct{
    TableContainer* hash_table;
    size_t TableMap_Size;
}TableMap;

size_t hash(const char* TableName, size_t mod);
TableMap* init_TableMap(size_t size);

void insertTable(TableMap* T_Map, Table* table);
Table* findTable(TableMap* T_Map, char* TableName);
void FreeTableMap(TableMap* T_Map);
void DropTable(TableMap* T_Map, char* TableName);
