#ifndef JRD_TABLEMAP_H
#define JRD_TABLEMAP_H
#include "custom_table.h"
#include "common.h"

// key - The Table Name
// value - The Table Structure (Pointer on the structure)


size_t hash(const char* TableName, size_t mod);
TableMap* init_TableMap(size_t size);

void insertTable(TableMap* T_Map, Table* table);
Table* findTable(TableMap* T_Map, char* TableName);
void FreeTableMap(TableMap* T_Map);
void DropTable(TableMap* T_Map, char* TableName);
#endif