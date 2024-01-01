# SimpleSQL-like_DB

### To-do (For myself)
- Write the virtual machine
- Write the simple virtual machine support
- Implement the B-tree storing data as in disk as in the memory layout

### Current state
- Written REPL to interact with the user
- Written Buffer_Input to be used to send the SQL-queries
- Written tokenizer to divide the buffer input into tokens
- Written the custom table creation support ?
- Written the storage about the custom table skeleton info ?


### To install on your machine
- git clone https://github.com/jali22ramazan22/SimpleSQL-like_DB.git
- cd SimpleSQL-like_DB/ && mkdir build
- cd build && cmake --build .
- ./JRD


## The origins
Inspired by https://cstack.github.io/db_tutorial/ tutorial. However, the tutorial goes along the hardcoded table support. So I've been trying to implement the dynamic table support for
this project. I don't know will i use or write the parser, but i've decided to write and hope that user guarantees to write the proper input.
