#include "custom_table.h"


static void init_column(TableColumn* column, int datatype, char* column_name) {
    strcpy(column->column_name, column_name);
    column->type = datatype;
    // column->head = NULL;
}


TableColumn* create_table(int* datatypes, char** column_names, int column_count){
    if(datatypes == NULL || column_names == NULL)
        return NULL;
    TableColumn* new_table = (TableColumn*)malloc(sizeof(TableColumn) * column_count + 1);
    if (new_table == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }
    init_column(&new_table[0], INT, "ID");
    for(int i = 1; i < column_count + 1; ++i){
        init_column(&new_table[i], datatypes[i], column_names[i]);
    }
    return new_table;
}


