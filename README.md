# SimpleSQL-like_DB

### To-do (For myself)
- Understand the architecture of SQLite3
- Write the virtual machine
- Write the custom database support
- Write the simple virtual machine support
- Implement the B-tree storing data as in disk as in the memory layout

### Current state
- Written REPL to interact with the user
- Written Buffer_Input to be used to send the SQL-queries
- Written tokenizer to divide the buffer input into tokens


### To install on your machine
- git clone https://github.com/jali22ramazan22/SimpleSQL-like_DB.git
- cd SimpleSQL-like_DB/ && mkdir build
- cd build && cmake --build .
- ./JRD
