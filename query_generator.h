
#ifndef JRD_QUERY_GENERATOR_H
#define JRD_QUERY_GENERATOR_H
#include "common.h"
#include "TableMap.h"
#include "custom_table.h"
#include "utils.h"
void QueryCreate(char** tokenized_input, size_t tokens_count, TableMap* T_Map);
void* QueryInsert(char** tokenized_input, size_t tokens_count, TableMap* T_Map);
void QuerySelect(char** tokenized_input, size_t tokens_count, TableMap* T_Map);
#endif //JRD_QUERY_GENERATOR_H
