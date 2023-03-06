#include "CodeObject.hpp"
#include "ByteCode.hpp"

#include "../PrettyPrint.hpp"
#include <iostream>
#include <iomanip>
#include <unordered_map>


namespace python
{

    std::unordered_map<unsigned char, const char*> EnumDict = {
        {1, "POP_TOP"},
        {2, "ROT_TWO"},
        {3, "ROT_THREE"},
        {4, "DUP_TOP"},
        {11, "UNARY_NEGATIVE"},
        {20, "BINARY_MULTIPLY"},
        {22, "BINARY_MODULO"},
        {25, "BINARY_SUBSCR"},
        {21, "BINARY_DIVIDE"},
        {23, "BINARY_ADD"},
        {24, "BINARY_SUBTRACT"},
        {55, "INPLACE_ADD"},
        {54, "STORE_MAP"},
        {56, "INPLACE_SUBSTRACT"},
        {57, "INPLACE_MULTIPLY"},
        {58, "INPLACE_DIVIDE"},
        {59, "INPLACE_MODULO"},
        {60, "STORE_SUBSCR"},
        {61, "DELETE_SUBSCR"},
        {68, "GET_ITER"},
        {71, "PRINT_ITEM"},
        {72, "PRINT_NEWLINE"},
        {80, "BREAK_LOOP"},
        {82, "LOAD_LOCALS"},
        {83, "RETURN_VALUE"},
        {86, "YIELD_VALUE"},
        {87, "POP_BLOCK"},
        {88, "END_FINALLY"},
        {89, "BUILD_CLASS"},
        //{90, "HAVE_ARGUMENT"},
        {90, "STORE_NAME"},
        {92, "UNPACK_SEQUENCE"},
        {93, "FOR_ITER"},
        {95, "STORE_ATTR"},
        {97, "STORE_GLOBAL"},
        {99, "DUP_TOPX"},
        {100, "LOAD_CONST"},
        {101, "LOAD_NAME"},
        {102, "BUILD_TUPLE"},
        {103, "BUILD_LIST"},
        {105, "BUILD_MAP"},
        {106, "LOAD_ATTR"},
        {107, "COMPARE_OP"},
        {108, "IMPORT_NAME"},
        {109, "IMPORT_FROM"},
        {110, "JUMP_FORWARD"},
        {111, "JUMP_IF_FALSE_OR_POP"},
        {113, "JUMP_ABSOLUTE"},
        {114, "POP_JUMP_IF_FALSE"},
        {115, "POP_JUMP_IF_TRUE"},
        {116, "LOAD_GLOBAL"},
        {119, "CONTINUE_LOOP"},
        {120, "SETUP_LOOP"},
        {121, "SETUP_EXCEPT"},
        {122, "SETUP_FINALLY"},
        {124, "LOAD_FAST"},
        {125, "STORE_FAST"},
        {130, "RAISE_VARARGS"},
        {131, "CALL_FUNCTION"},
        {132, "MAKE_FUNCTION"},
        {134, "MAKE_CLOSURE"},
        {135, "LOAD_CLOSURE"},
        {136, "LOAD_DEREF"},
        {137, "STORE_DEREF"},
        {140, "CALL_FUNCTION_VAR"},
    };



    void print_array_list(ArrayList<Object*>* list)
    {

        if (!list)
        {
            std::cout << "Empty List\n";
            return;
        }
        
        std::cout << "[";
        for (int i = 0; i < list->size(); ++i)
        {
            auto obj = list->get(i);
            if (obj)
            {
                if (i) std::cout << ", ";
                obj->show();
            }
            else
            {
                std::cout << "nullptr\n";
            }
        }
        std::cout << "]";
    }

    void print_byte_code(String* codes, int level)
    {
        const char* data = codes->c_str();
        const int length = codes->length();

        std::string prefix;
        prefix.append("", level);

        // for (int i = 0; i < length; ++i)
        // {
            // std::cout << std::format("{:0>2d} ", (int)data[i] & 0xFF);
            // std::cout << std::dec << std::setw(2) << std::setfill('0') << ((int)data[i] & 0xFF) << ' ';
        // }
        // std::cout << '\n';


        for (int i = 0; i < length; ++i)
        {
            // std::cout << std::format("{:0>2x}", (int)data[i] & 0xFF);
            std::cout << std::hex << std::setw(2) << std::setfill('0') << ((int)data[i] & 0xFF);
        }
        std::cout << std::dec;
    }

    void translate_byte_code(String* codes, int level)
    {
        const char* data = codes->c_str();
        const int length = codes->length();

        std::string prefix;
        prefix.append('\t', level);

        for (int i = 0; i < length;)
        {
            auto code_type = data[i] & 0xFF;
            PYTHON_ASSERT(EnumDict.count(code_type));
            // std::cout << std::format("\t\t{} \t{}\n", i, EnumDict[code_type]);
            std::cout << prefix << "\t\t" << i << " \t" << EnumDict[code_type] << "\n";
            if (code_type >= ByteCode::HAVE_ARGUMENT)
            {
                i += 3;
            }
            else
            {
                i++;
            }
        }
    }

    void show_code_object(CodeObject* code, int level)
    {

        print_label_style("argcount", code->co_argcount, level);
        print_label_style("nlocals", code->co_nlocals, level);
        print_label_style("stack_size", code->co_stacksize, level);
        print_label_style("flag", code->co_flags, level);

        std::cout << "<code>";
        print_byte_code(code->co_code, level + 1);
        std::cout << "</code>\n";

        std::cout << "<consts>\n";
        print_array_list(code->co_consts);
        std::cout << "\n</consts>\n";
    
        std::cout << "<co_names>";
        code->co_name->show();
        std::cout << "</co_names>";
        std::cout << '\n';

        std::cout << "<co_varnames>";
        print_array_list(code->co_varnames);
        std::cout << "</co_varnames>\n";

        std::cout << "<co_freevars>";
        print_array_list(code->co_freevars);
        std::cout << "</co_freevars>\n";

        std::cout << "<co_cellvars>";
        print_array_list(code->co_cellvars);
        std::cout << "</co_cellvars>\n";

        std::cout << "<names>";
        print_array_list(code->co_names);
        std::cout << "</names>\n";

        std::cout << "<co_filename>";
        code->co_filename->show();
        std::cout << "</co_filename>\n";

        std::cout << "<co_name>";
        code->co_name->show();
        std::cout << "</co_name>\n";

        translate_byte_code(code->co_code, level);

    }
}
