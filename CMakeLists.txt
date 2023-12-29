cmake_minimum_required(VERSION 3.22)
project(JRD)
add_subdirectory(tests)
enable_testing()
add_executable(JRD main.c utils.c tokenizer.c custom_table.c
        TableMap.c
        TableMap.h)
target_include_directories(JRD PRIVATE src)
set_target_properties(JRD PROPERTIES C_STANDARD 11)
