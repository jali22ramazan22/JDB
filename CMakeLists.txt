cmake_minimum_required(VERSION 3.22)
project(JRD)
add_executable(JRD main.c utils.c tokenizer.c custom_table.c)
target_include_directories(JRD PRIVATE src)
add_custom_target(lemon
    COMMAND make -C ${CMAKE_SOURCE_DIR}/lemon
    COMMENT "Building Lemon parser generator"
)
add_dependencies(JRD lemon)
set_target_properties(JRD PROPERTIES C_STANDARD 11)
