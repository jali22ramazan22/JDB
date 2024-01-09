#include "query_generator.h"

// "create table <NAME> with columns <NAME> <DATATYPE>, ..."
// "insert in table <NAME> values <VALUE_1>, <VALUE_2> ..."
// "select <COLUMN 1> <COLUMN 2> ...  from table <NAME> where <EXPRESSION> "

//it's guaranteed that the input is correct because of lack of parser

int CastStringToDatatype(char* expected_datatype){
    to_lower_string(expected_datatype);
    if(strcmp(expected_datatype, "int") == 0){
        return INT;
    }
    else if(strcmp(expected_datatype, "varchar") == 0){
        return VARCHAR;
    }
    else if(strcmp(expected_datatype, "double") == 0){
        return DOUBLE;
    }
    else if(strcmp(expected_datatype, "bool") == 0 || (strcmp(expected_datatype, "boolean")) == 0){
        return BOOLEAN;
    }
}

void FreeDataTypeValue(void* value, int datatype) {
    if(value == NULL){
        return;
    }
    if(datatype == INT){
        int* int_to_free = (int*)value;
        free(int_to_free);
    }
    else if(datatype == DOUBLE){
        double* double_to_free = (double*)value;
        free(double_to_free);
    }
    else if(datatype == VARCHAR){
        char* char_to_free = (char*)value;
        free(char_to_free);
    }
    else if(datatype == BOOLEAN){
        bool* bool_to_free = (bool*)value;
        free(bool_to_free);
    }
}

void* CastStringValueToDatatypeValue(char* expected_value, int datatype) {
    void* value = NULL;
    if (datatype == INT) {
        int* data = (int*)malloc(sizeof(int));
        if (data != NULL) {
            *data = (int)strtol(expected_value, NULL, 10);
            value = data;
        }
    }
    else if (datatype == DOUBLE) {
        double* data = (double*)malloc(sizeof(double));
        if (data != NULL) {
            *data = (double)strtod(expected_value, NULL);
            value = data;
        }
    }
    else if (datatype == VARCHAR) {
        char* data = strdup(expected_value);
        if (data != NULL) {
            value = data;
        }
    }
    else if(datatype == BOOLEAN){
        bool* data = (bool*)malloc(sizeof(bool));
        if(data != NULL){
            if(strcmp(expected_value, "false") == 0){
                *data = false;
            }
            else{
                *data = true;
            }
            value = data;
        }
    }
    return value;
}


void QueryCreate(char** tokenized_input, size_t tokens_count, TableMap* T_Map){
    if(tokenized_input == NULL)
        return;
    size_t columns_count = (tokens_count - 5) / 2;
    char TableName[MAX_NAME];
    char** ColumnNames = (char**)malloc(sizeof(char*)*columns_count);
    int datatypes[columns_count];

    int j = 0;
    for(int i = 5; i < tokens_count - 1; i += 2){
        if(i < tokens_count){
            ColumnNames[j] = strdup(tokenized_input[i]);
            datatypes[j] = CastStringToDatatype(tokenized_input[i+1]);
            j++;
        }
    }
    Table* NewTableScheme = create_table(datatypes, (const char **) ColumnNames, (int)columns_count, tokenized_input[2]);
    insertTable(T_Map, NewTableScheme);
    for(int i = 0; i < columns_count; ++i){
        free(ColumnNames[i]);
    }
    free(ColumnNames);
}


void* QueryInsert(char** tokenized_input, size_t tokens_count, TableMap* T_Map){
    if(tokenized_input == NULL)
        return NULL;
    Table* TableScheme = findTable(T_Map, tokenized_input[3]);
    if(TableScheme == NULL){
        printf("The table '%s' is not found\n", tokenized_input[3]);
        return NULL;
    }
    void** values = NULL;
    if(TableScheme->column_count > 0){
       values = (void*)malloc(TableScheme->column_count * sizeof(void*));
    }

    for(int i = 0; i < TableScheme->column_count; ++i){
        if(values != NULL && TableScheme->columns != NULL){
            values[i] = CastStringValueToDatatypeValue(tokenized_input[i+5], TableScheme->columns[i].type);
        }
    }
    Row* new_record = InitRecord(T_Map, tokenized_input[3], values);
    void* serialized_row = serialize_row(new_record, T_Map, tokenized_input[3]);
    if(values != NULL){
        for(int i = 0; i < TableScheme->column_count; ++i){
            FreeDataTypeValue(values[i], TableScheme->columns[i].type);
        }
        free(values);

    }
    return serialized_row;
}

//Difficult to implement without B_tree implementation
void QuerySelect(char** tokenized_input, size_t tokens_count, TableMap* T_Map){
    /* if(tokenized_input == NULL)
        return;
    char* TableName; int i;
    for(i = 1; i < tokens_count; ++i){
        if(strcmp("from", tokenized_input[i++]) == 0){
           break;
        }
    }
    TableName = strdup(tokenized_input[i]);
    Table* TableScheme = findTable(T_Map, TableName);

    free(TableName);*/
}

