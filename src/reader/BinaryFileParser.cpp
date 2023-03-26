// /*
//     Reference:
//         https://kdr2.com/tech/main/1012-pyc-format.html
//         https://docs.python.org/release/2.7/library/dis.html#python-bytecode-instructions
// */

// #include "BinaryFileParser.hpp"
// #include "../objects/Integer.hpp"
// #include "../objects/Float.hpp"
// #include "../objects/List.hpp"
// #include "../objects/Universe.hpp"
// #include "../Python.hpp"

// // #include <format>
// #include "../PrettyPrint.hpp"
// #include <iostream>

// // https://github.com/python/cpython/blob/main/Python/marshal.c
// #define TYPE_NULL               '0'
// #define TYPE_NONE               'N'
// #define TYPE_FALSE              'F'
// #define TYPE_TRUE               'T'
// #define TYPE_STOPITER           'S'
// #define TYPE_ELLIPSIS           '.'
// #define TYPE_INT                'i'
// #define TYPE_INT64              'I'
// #define TYPE_FLOAT              'f'
// #define TYPE_BINARY_FLOAT       'g'
// #define TYPE_COMPLEX            'x'
// #define TYPE_BINARY_COMPLEX     'y'
// #define TYPE_LONG               'l'
// #define TYPE_STRING             's'
// #define TYPE_INTERNED           't'
// #define TYPE_REF                'r'
// #define TYPE_TUPLE              '('
// #define TYPE_LIST               '['
// #define TYPE_DICT               '{'
// #define TYPE_CODE               'c'
// #define TYPE_UNICODE            'u'
// #define TYPE_UNKNOWN            '?'
// #define TYPE_SET                '<'
// #define TYPE_FROZENSET          '>'
// #define FLAG_REF                '\x80' 

// #define TYPE_ASCII              'a'
// #define TYPE_ASCII_INTERNED     'A'
// #define TYPE_SMALL_TUPLE        ')'
// #define TYPE_SHORT_ASCII        'z'
// #define TYPE_SHORT_ASCII_INTERNED 'Z'

// namespace python 
// {

//     CodeObject* BinaryFileParser::get_code_object()
//     {
//         int argcount = this->file_stream->read_int();

//         int nlocals = this->file_stream->read_int();
//         int stack_size = this->file_stream->read_int();
//         int flag = this->file_stream->read_int();

//         // std::cout << std::format("nlocals = {}, stacksize = {} and flags = {:0x}\n", nlocals, stack_size, flag);
//         // print_label_style("argcount", argcount);
//         // print_label_style("nlocals", nlocals);
//         // print_label_style("stack_size", stack_size);
//         // print_label_style("flag", flag);

//         //auto code_obj = (CodeObject*)::operator new(sizeof(CodeObject));
//         auto code_obj = new CodeObject();


//         code_obj->co_argcount = argcount;
//         code_obj->co_nlocals = nlocals;
//         code_obj->co_stacksize = stack_size;
//         code_obj->co_flags = flag;

//         code_obj->co_code = this->get_byte_codes();
//         code_obj->co_consts = this->get_consts();
//         code_obj->co_names = this->get_names();
//         code_obj->co_varnames = this->get_var_names();
//         code_obj->co_freevars = this->get_free_vars();
//         code_obj->co_cellvars = this->get_cell_vars();

//         code_obj->co_filename = this->get_file_name();
//         code_obj->co_name = this->get_name();
//         code_obj->co_firstlineno = this->file_stream->read_int();
//         code_obj->co_lnotab = this->get_no_table();
        
//         return code_obj;
//     }

//     String* BinaryFileParser::get_byte_codes()
//     {
//         char c = this->file_stream->read();
//         PYTHON_ASSERT(c == TYPE_STRING);
//         return this->get_string();
//     }

//     String* BinaryFileParser::get_string()
//     {
//         const int length = this->file_stream->read_int();
//         String* s = new String(this->file_stream->data(), length);
//         this->file_stream->skip(length);
//         return s;
//     }

//     ArrayList<Object*>* BinaryFileParser::get_consts()
//     {
//         if (this->file_stream->read() == TYPE_TUPLE)
//         {
//             return this->get_tuple();
//         }
//         this->file_stream->unread();
//         return nullptr;
//     }

//     ArrayList<Object*>* BinaryFileParser::get_tuple()
//     {
//         const int length = this->file_stream->read_int();
//         ArrayList<Object*>* list = new ArrayList<Object*>(length);
//         String* str = nullptr;
//         CodeObject* codeobj = nullptr;
//         for (int i = 0; i < length; ++i)
//         {
//             char obj_type = this->file_stream->read();
//             switch (obj_type)
//             {
//             case TYPE_CODE:
//                 // std::cout << "got a code object\n";
//                 codeobj = this->get_code_object();
//                 // std::cout << "end a code object\n";
//                 //show_code_object(codeobj);
//                 list->add(codeobj);
//                 break;
//             case TYPE_INT:
//                 list->add(new Integer(this->file_stream->read_int()));
//                 break;
//             case TYPE_BINARY_FLOAT:
//                 list->add(new Float(this->file_stream->read_double()));
//                 break;
//             case TYPE_NONE:
//                 list->add(Universe::None);
//                 break;
//             case 't':
//                 str = this->get_string();
//                 list->add(str);
//                 this->string_table.add(str);
//                 break;
//             case 's':
//                 list->add(this->get_string());
//                 break;
//             case 'R': // ?
//                 list->add(this->string_table.get(this->file_stream->read_int()));
//                 break;
//             case '(':
//                 {
//                     auto tuples = get_tuple();
//                     auto ls = new List();
//                     for (int i = 0; i < tuples->size(); ++i)
//                     {
//                         ls->append(ls->get(i));
//                     }
//                     list->add(ls);
//                     break;
//                 };
//             default:
//                 std::cout << "Parse tuple Error" << ", got unknown character " << obj_type << "\n";
//                 std::terminate();
//             }
//         }

//         return list;
//     }

//     ArrayList<Object*>* BinaryFileParser::get_cell_vars()
//     {
//         return this->get_consts();
//     }

//     ArrayList<Object*>* BinaryFileParser::get_free_vars()
//     {
//         return this->get_consts();
//     }

//     ArrayList<Object*>* BinaryFileParser::get_var_names()
//     {
//         return this->get_consts();
//     }

//     ArrayList<Object*>* BinaryFileParser::get_names()
//     {
//         return this->get_consts();
//     }

//     String* BinaryFileParser::get_no_table()
//     {
//         char ch = this->file_stream->read();
        
//         if (ch != 's' && ch != 't') {
//             this->file_stream->unread();
//             return nullptr;
//         }
        
//         return this->get_string();
//     }

//     String* BinaryFileParser::get_file_name()
//     {
//         return this->get_name();
//     }

//     String* BinaryFileParser::get_name()
//     {
//         char ch = this->file_stream->read();

//         if (ch == 's') {
//             return this->get_string();
//         }
//         else if (ch == 't') {
//             String* str = this->get_string();
//             this->string_table.add(str);
//             return str;
//         }
//         else if (ch == 'R') {
//             return this->string_table.get(this->file_stream->read_int());
//         }

//         return nullptr;
//     }

//     BinaryFileParser::BinaryFileParser(BufferedInputStream* stream) : file_stream(stream)
//     {
//     }

//     CodeObject* BinaryFileParser::parse()
//     {
//         int magic_number = this->file_stream->read_int();
//         // std::cout << std::format("magic number is {:0x}\n", magic_number);
//         std::cout << "<magic number>" << std::hex << magic_number << "</magic number>\n";

//         int moddata = this->file_stream->read_int();
//         // std::cout << std::format("moddate is {:0x}\n", moddata);
//         std::cout << "<moddate>" << std::hex << moddata << "<moddate>\n";
//         std::cout << std::dec;
    
//         char object_type = this->file_stream->read();

//         if (object_type == 'c')
//         {
//             CodeObject* result = this->get_code_object();
//             show_code_object(result, 0);
//             std::cout << "parse OK! \n";

//             return result;
//         }

//         return nullptr;
//     }
// }
