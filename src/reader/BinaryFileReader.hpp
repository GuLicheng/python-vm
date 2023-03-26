
#pragma once

#include "../objects/CodeObject.hpp"
#include "../objects/Object.hpp"
#include "../objects/List.hpp"

#include <fstream>

namespace python
{
    class BinaryFileReader
    {
        std::string m_bytes;

        std::vector<String*> m_string_table;

        int m_index;

        char read();

        int read_int();

        double read_double();

        void unread();

        void skip(int n);

        CodeObject* get_code_object();

        String* get_byte_codes();

        String* get_string();

        List* get_consts();

        List* get_tuple();

        List* get_cell_vars();

        List* get_free_vars();

        List* get_var_names();

        List* get_names();

        String* get_no_table();

        String* get_file_name();

        String* get_name();

    public:

        BinaryFileReader() = default;

        CodeObject* parse(const char* filename);

        void show_hex_code() const;
    };
}


