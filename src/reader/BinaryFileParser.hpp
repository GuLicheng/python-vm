
// #pragma once

// #include "BufferedInputStream.hpp"
// #include "../objects/CodeObject.hpp"
// #include "../objects/Object.hpp"
// #include "../objects/List.hpp"
// #include "../ArrayList.hpp"


// namespace python
// {
//     class BinaryFileParser
//     {
//         BufferedInputStream* file_stream;

//         ArrayList<String*> string_table;

//         CodeObject* get_code_object();

//         String* get_byte_codes();

//         String* get_string();

//         ArrayList<Object*>* get_consts();

//         ArrayList<Object*>* get_tuple();

//         ArrayList<Object*>* get_cell_vars();

//         ArrayList<Object*>* get_free_vars();

//         ArrayList<Object*>* get_var_names();

//         ArrayList<Object*>* get_names();

//         String* get_no_table();

//         String* get_file_name();

//         String* get_name();

//     public:

//         BinaryFileParser(BufferedInputStream* stream);

//         CodeObject* parse();
//     };
// }


