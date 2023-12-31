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
            return 64;
        case DOUBLE:
            return 8;
        default:
            return 0;
    }
}




uint32_t* get_offsets(Table* table) {
    uint32_t* offsets = (uint32_t*)malloc(sizeof(uint32_t) * (table->column_count + 1));
    if(offsets == NULL){
        return NULL;
    }
    offsets[0] = 8;
    for (size_t i = 0; i < table->column_count; ++i) {
        offsets[i + 1] = get_size(table->columns[i].type);
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
    new_node->type = type;
    new_node->next = NULL;
    if (row->head == NULL) {
        row->head = new_node;
    } else {
        RowNode* current = row->head;
        while (current->next != NULL) {
            current = current->next;
        }

        current->next = new_node;
    }

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
void print_row(Row* row) {
    if (row == NULL) {
        printf("Row is NULL\n");
        return;
    }

    printf("Row Size: %d\n", row->size);

    RowNode* current = row->head;

    while (current != NULL) {
        switch (current->type) {
            case INT:
                printf("Type: INT, Value: %d\n", *((int*)current->data));
                break;
            case VARCHAR:
                printf("Type: VARCHAR, Value: %s\n", (char*)current->data);
                break;
            case DOUBLE:
                printf("Type: DOUBLE, Value: %lf\n", *((double*)current->data));
                break;
            default:
                printf("Unknown Type\n");
                break;
        }

        current = current->next;
    }
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
RowNode* extractNode(Row* row, int index){
    if(row->size < index){
        printf("Out of List\n");
        return NULL;
    }
    RowNode* it = row->head;
    for(int i = 0; i < index; ++i){
        it = it->next;
    }
    return it;
}


Row* InitRecord(TableMap* T_Map, char* TableName, void** values) {
    Table* TableScheme = findTable(T_Map, TableName);
    if (TableScheme == NULL) {
        return NULL;
    }

    Row* new_record = create_row();
    if (new_record == NULL) {
        return NULL;
    }

    for (int i = 0; i < TableScheme->column_count; ++i) {
        add_to_row(new_record, values[i], TableScheme->columns[i].type);
    }

    return new_record;
}


void* serialize_row(Row* Record, TableMap* T_Map, char* TableName) {
    // Finding concrete skeleton of Table
    Table* TableScheme = findTable(T_Map, TableName);
    if (TableScheme == NULL) {
        return NULL;
    }

    size_t hashIndex = hash(TableName, T_Map->TableMap_Size);
    if (T_Map->hash_table[hashIndex].table_ptr == NULL) {
        return NULL;
    }

    uint32_t* offsets = get_offsets(TableScheme);
    uint32_t bytesToAlloc = 0;

    // Calculate total bytes to allocate
    for (int i = 0; i <= TableScheme->column_count; ++i) {
        bytesToAlloc += offsets[i];
    }

    // Allocate memory for serialized data
    void* serialized_data = malloc(bytesToAlloc);
    if (serialized_data == NULL) {
        free(offsets);
        return NULL;  // Failed to allocate memory
    }

    // Copy hashIndex to the beginning
    memcpy(serialized_data, (const void*)&hashIndex, sizeof(hashIndex));
    int totalOffset = sizeof(hashIndex);  // Start from the next offset

    for (int i = 1; i <= TableScheme->column_count; ++i) {
        RowNode* columnPointer = extractNode(Record, i-1);

        // Check if columnPointer is not NULL before accessing its data
        if (columnPointer != NULL) {
            memcpy(serialized_data + totalOffset, columnPointer->data, get_size(columnPointer->type));
            totalOffset += (int)offsets[i];
        }
    }
    free(offsets);
    return serialized_data;
}

Row* deserialize_row(TableMap* T_Map, void* serialized_data) {
    size_t hashIndex = *((size_t*)serialized_data);
    Table* TableScheme = findTable(T_Map, T_Map->hash_table[hashIndex].table_ptr->tableName);
    uint32_t* offsets = get_offsets(TableScheme);

    Row* recovered_record = create_row();
    int totalOffset = 8;
    for (int i = 1; i <= TableScheme->column_count; ++i) {
        add_to_row(recovered_record, (serialized_data + totalOffset), TableScheme->columns[i-1].type);
        totalOffset += (int)offsets[i];
    }
    free(offsets);
    return recovered_record;
}