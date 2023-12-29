#include "custom_table.h"

/* This module trying to implement the creation in the RAM the custom table frame

    The tokenizer + parser goes here and creates the table.

    To do: collect it into the abstract the data part with offset info and store it as a 'page' in RAM


*/

/* The table structure creation */

static void init_column(TableColumn* column, const Datatype datatype, const char* column_name) {
    column->type = datatype;
    strncpy(column->column_name, column_name, sizeof(column->column_name) - 1);
    column->column_name[sizeof(column->column_name) - 1] = '\0';  
}



Table* create_table(const int* Datatypes, const char** ColumnNames, const int ColumnCount, const char* TableName) {
    if (Datatypes == NULL || ColumnNames == NULL)
        return NULL;
    Table* new_table = (Table*)malloc(sizeof(Table));
    new_table->columns = (TableColumn*)malloc(sizeof(TableColumn) * ColumnCount);
    new_table->column_count = ColumnCount;
    for (int i = 0; i < ColumnCount; ++i) {
        init_column(&new_table->columns[i], Datatypes[i], ColumnNames[i]);
    }
    strcpy(new_table->tableName, TableName);
    return new_table;
}


void free_table(Table* table){
    free(table->columns);
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

uint32_t* get_offsets(Table* table) {

    uint32_t* offsets = (uint32_t*)malloc(sizeof(uint32_t) * table->column_count + 1);
    offsets[0] = 4; // for hash code of table
    for (size_t i = 1; i < table->column_count + 1; ++i) {
        if (i > 1){
            offsets[i] = offsets[i - 1] + get_size(table->columns[i].type);
        } 
        else{
            offsets[i] = get_size(table->columns[i].type);
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
 





