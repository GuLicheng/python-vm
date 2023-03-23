#include "../objects/String.hpp"
#include "../objects/CodeObject.hpp"
#include "../objects/List.hpp"

#include <fstream>

namespace python3
{
    using python::String;
    using python::PythonList;
    using python::CodeObject;

    class UnicodeParser
    {
        PythonList string_table;

        CodeObject* get_code_object();

        std::string get_byte_codes();

        std::string get_string();

        PythonList get_consts();

        PythonList get_tuple();

        PythonList get_cell_vars();

        PythonList get_free_vars();

        PythonList get_var_names();

        PythonList get_names();

        std::string get_no_table();

        std::string get_file_name();

        std::string get_name();

    public:

        UnicodeParser(const char* file);

        CodeObject* parse();
    };
}
