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
    TableColumn* new_table = (TableColumn*)malloc(sizeof(TableColumn) * (column_count));
    if (new_table == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    for(int i = 0; i < column_count; ++i){
        init_column(&new_table[i], datatypes[i], column_names[i]);
    }
    return new_table;
}

void free_table(TableColumn* table){
    free(table);
}

uint32_t get_size(Datatype type){
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

uint32_t* get_offsets(TableColumn* table, size_t column_count) {
    uint32_t* offsets = (uint32_t*)malloc(sizeof(uint32_t) * column_count + 1);
    offsets[0] = 4; // for hash code of table
    if (offsets == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 1; i < column_count + 1; ++i) {
        if (i > 1){
            offsets[i] = offsets[i - 1] + get_size(table[i].type);
        } 
        else{
            offsets[i] = get_size(table[i].type);
        }
    }
    return offsets;
}


Row* create_row() {
    Row* new_row = (Row*)malloc(sizeof(Row));
    new_row->size = 0;
    new_row->head = NULL;
    return new_row;
}
 
void add_to_row(Row* row, void* data, Datatype type) {
    RowNode* new_node = (RowNode*)malloc(sizeof(RowNode));
    new_node->data = data;
    new_node->next = row->head;
    new_node->type = type;
    row->head = new_node;
    row->size++;
}
 
void* remove_from_row(Row* row) {
    if (row->size == 0) {
        return NULL;
    }
    RowNode* node_to_remove = row->head;
    void* data = node_to_remove->data;
    row->head = node_to_remove->next;
    free(node_to_remove);
    row->size--;
    return data;
}
 
void free_row(Row* row) {
    RowNode* current_node = row->head;
    while (current_node != NULL) {
        RowNode* next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(row);
}
 



void serialize_row(const char* tableName, const void* values, TableNode* TableMap,
     __ssize_t TableMap_size, void* destination) {

    TableNode* insertionTable = find_TableMap(TableMap, TableMap_size, tableName);
    if (insertionTable == NULL) {
        fprintf(stderr, "Table not found: %s\n", tableName);
        return;
    }
    int tableHashCode = hash_function(insertionTable->tableName, TableMap_size);
    memcpy(destination, &tableHashCode, TableMap_size);
    uint32_t* offsets = get_offsets(insertionTable->table_ptr, insertionTable->column_count);
    if (offsets == NULL) {
        perror("Failed to get offsets");
        return;
    }
    for (size_t i = 0; i < insertionTable->column_count; ++i) {
        size_t size = get_size(insertionTable->table_ptr[i].type);
        if (size > 0) {
            memcpy(destination + offsets[i], values + offsets[i], size);
        } 
        else {
            fprintf(stderr, "Invalid size for column %zu\n", i);
        }
    }
    free(offsets);
}


Row* deserialize_row(const void* source, TableNode* TableMap, __ssize_t TableMap_size){
    int hashcode = *((int*)source);
    Row* new_row = create_row();
    uint32_t* offsets = get_offsets(TableMap[hashcode].table_ptr, TableMap[hashcode].column_count);
    if (offsets == NULL) {
        perror("Failed to get offsets");
        return NULL;
    }
    for(int i = 1; i < TableMap[hashcode].column_count + 1; ++i){
        add_to_row(new_row, source+offsets[i], TableMap[hashcode].table_ptr[i].type);
    }
    free(offsets);
    return new_row;
}


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
        tableMap[i].column_count = 0;
        tableMap[i].next = NULL;
    }
    return tableMap;
}

void insertTable(TableNode* tableMap, __ssize_t TableMap_size, const char* tableName, 
    const int column_count,TableColumn* table) {
    
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
    iterator->next->column_count = column_count;
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