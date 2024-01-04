#include "TableMap.h"
//TODO: write the rehashing algorithm

size_t hash(const char* key, size_t TableSize) {
    const size_t c1 = 71;
    const size_t c2 = 37;
    size_t hash_value = 0;
    for (size_t i = 0; key[i] != '\0'; ++i) {
        hash_value = (hash_value * c1 + (size_t)key[i] + c2 * i) % TableSize;
    }
    return hash_value;
}

TableMap* init_TableMap(size_t size){
    if(size <= 0){
       return NULL;
    }
    TableMap* T_Map = (TableMap*)malloc(sizeof(TableMap));
    if(T_Map == NULL){
        perror("Unable to allocate 'TableMap'\n");
        exit(EXIT_FAILURE);
    }
    T_Map->TableMap_Size = size;
    T_Map->hash_table = (TableContainer*)malloc(sizeof(TableContainer) * size);
    if (T_Map->hash_table == NULL) {
        free(T_Map);
        perror("Failed to allocate memory for hash_table");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < size; ++i){
        T_Map->hash_table[i].table_ptr = NULL;
    }
    return T_Map;
}

void insertTable(TableMap* T_Map, Table* table){
    size_t hashIndex = hash(table->tableName, T_Map->TableMap_Size);
    T_Map->hash_table[hashIndex].table_ptr = table;
}

Table* findTable(TableMap* T_Map, char* TableName){
    size_t hashIndex = hash(TableName, T_Map->TableMap_Size);
    if (T_Map->hash_table[hashIndex].table_ptr == NULL){
        printf("The given table '%s' does not exist.\n", TableName);
        return NULL;
    }
    return T_Map->hash_table[hashIndex].table_ptr;
}

void DropTable(TableMap* T_Map, char* TableName){
    Table* DroppingTable = findTable(T_Map, TableName);
    free_table(DroppingTable);
}
void PrintAllTables(TableMap* T_Map){
    if(T_Map == NULL){
        return;
    }
    int any = 0;
    for(int i = 0; i < T_Map->TableMap_Size; ++i){
        if(T_Map->hash_table[i].table_ptr != NULL){
            any = 1;
            printf("Found table '%s' with columns ", T_Map->hash_table[i].table_ptr->tableName);
            for(int j = 0; j < T_Map->hash_table[i].table_ptr->column_count; ++j) {
                printf("'%s', ", T_Map->hash_table[i].table_ptr->columns[j].column_name);
            }
        }

    }
    printf("\n");
    if(any == 0){
        printf("No table has been found\n");
    }
}
void FreeTableMap(TableMap* T_Map){
    for(int i = 0; i < T_Map->TableMap_Size; ++i){
        if(T_Map->hash_table[i].table_ptr != NULL){
            free_table(T_Map->hash_table[i].table_ptr);
        }
    }
    free(T_Map);
}