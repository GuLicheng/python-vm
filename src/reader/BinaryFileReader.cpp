#include "BinaryFileReader.hpp"
#include "../PrettyPrint.hpp"
#include "../objects/Integer.hpp"
#include "../objects/Float.hpp"
#include "../objects/Universe.hpp"

#include <iterator>
#include <bit>

static_assert(std::endian::native == std::endian::little);

// https://github.com/python/cpython/blob/main/Python/marshal.c
#define TYPE_NULL               '0'
#define TYPE_NONE               'N'
#define TYPE_FALSE              'F'
#define TYPE_TRUE               'T'
#define TYPE_STOPITER           'S'
#define TYPE_ELLIPSIS           '.'
#define TYPE_INT                'i'
#define TYPE_INT64              'I'
#define TYPE_FLOAT              'f'
#define TYPE_BINARY_FLOAT       'g'
#define TYPE_COMPLEX            'x'
#define TYPE_BINARY_COMPLEX     'y'
#define TYPE_LONG               'l'
#define TYPE_STRING             's'
#define TYPE_INTERNED           't'
#define TYPE_REF                'r'
#define TYPE_TUPLE              '('
#define TYPE_LIST               '['
#define TYPE_DICT               '{'
#define TYPE_CODE               'c'
#define TYPE_UNICODE            'u'
#define TYPE_UNKNOWN            '?'
#define TYPE_SET                '<'
#define TYPE_FROZENSET          '>'
#define FLAG_REF                '\x80' 

#define TYPE_ASCII              'a'
#define TYPE_ASCII_INTERNED     'A'
#define TYPE_SMALL_TUPLE        ')'
#define TYPE_SHORT_ASCII        'z'
#define TYPE_SHORT_ASCII_INTERNED 'Z'

namespace python
{
    template <typename T>
    T read_basic_type(const char* buffer, int& index)
    {
        constexpr auto offset = sizeof(T);
        T i = *reinterpret_cast<const T*>(buffer + index);
        index += offset;
        return i;
    }

    // BinaryFileReader::BinaryFileReader()
    // {
    // }

    CodeObject* BinaryFileReader::parse(const char* filename)
    {
        // Open file
        std::fstream ifs (filename, std::ios::in | std::ios::binary);
        if (!ifs.is_open())
        {
            PYTHON_ASSERT("Error path");
            return nullptr;
        }

        // Reset state
        this->m_bytes = std::string(
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>()
        );
        this->m_index = 0;
        this->m_string_table.clear();

        // Try parse
        int magic_number = this->read_int();
        // std::cout << std::format("magic number is {:0x}\n", magic_number);
        std::cout << "<magic number>" << std::hex << magic_number << "</magic number>\n";

        int moddata = this->read_int();
        // std::cout << std::format("moddate is {:0x}\n", moddata);
        std::cout << "<moddate>" << std::hex << moddata << "<moddate>\n";
        std::cout << std::dec;
    
        char object_type = this->read();

        if (object_type == 'c')
        {
            CodeObject* result = this->get_code_object();
            show_code_object(result, 0);
            std::cout << "parse OK! \n";

            return result;
        }

        return nullptr;
    }

    void BinaryFileReader::show_hex_code() const
    {
        print_hex_code<>(this->m_bytes.data(), this->m_bytes.size());
    }

    char BinaryFileReader::read()
    {
        return this->m_bytes[this->m_index++];
    }

    int BinaryFileReader::read_int()
    {
        return read_basic_type<int>(this->m_bytes.data(), this->m_index);
    }

    double BinaryFileReader::read_double()
    {
        return read_basic_type<double>(this->m_bytes.data(), this->m_index);
    }

    void BinaryFileReader::unread()
    {
        this->m_index--;
    }

    void BinaryFileReader::skip(int n)
    {
        this->m_index += n;
        PYTHON_ASSERT(this->m_index <= this->m_bytes.size());
    }

    CodeObject* BinaryFileReader::get_code_object()
    {
        int argcount = this->read_int();

        int nlocals = this->read_int();
        int stack_size = this->read_int();
        int flag = this->read_int();

        // std::cout << std::format("nlocals = {}, stacksize = {} and flags = {:0x}\n", nlocals, stack_size, flag);
        // print_label_style("argcount", argcount);
        // print_label_style("nlocals", nlocals);
        // print_label_style("stack_size", stack_size);
        // print_label_style("flag", flag);

        //auto code_obj = (CodeObject*)::operator new(sizeof(CodeObject));
        auto code_obj = new CodeObject();


        code_obj->m_argcount = argcount;
        code_obj->m_nlocals = nlocals;
        code_obj->m_stacksize = stack_size;
        code_obj->m_flags = flag;

        code_obj->m_code = this->get_byte_codes();
        code_obj->m_consts = this->get_consts();
        code_obj->m_names = this->get_names();
        code_obj->m_varnames = this->get_var_names();
        code_obj->m_freevars = this->get_free_vars();
        code_obj->m_cellvars = this->get_cell_vars();

        code_obj->m_filename = this->get_file_name();
        code_obj->m_name = this->get_name();
        code_obj->m_firstlineno = this->read_int();
        code_obj->m_lnotab = this->get_no_table();
        
        return code_obj;
    }

    String* BinaryFileReader::get_byte_codes()
    {
        char c = this->read();
        PYTHON_ASSERT(c == TYPE_STRING);
        return this->get_string();
    }

    String* BinaryFileReader::get_string()
    {
        const int length = this->read_int();
        auto str = new String(this->m_bytes.data() + this->m_index, length);
        this->skip(length);
        return str;
    }

    List* BinaryFileReader::get_consts()
    {
        if (this->read() == TYPE_TUPLE)
        {
            return this->get_tuple();
        }
        this->unread();
        return nullptr;
    }

    List* BinaryFileReader::get_tuple()
    {
        const int length = this->read_int();
        Vector<Object*> list;
        String* str = nullptr;
        CodeObject* codeobj = nullptr;
        for (int i = 0; i < length; ++i)
        {
            char obj_type = this->read();
            switch (obj_type)
            {
            case TYPE_CODE:
                codeobj = this->get_code_object();
                list.emplace_back(codeobj);
                break;
            case TYPE_INT:
                list.emplace_back(new Integer(this->read_int()));
                break;
            case TYPE_BINARY_FLOAT:
                list.emplace_back(new Float(this->read_double()));
                break;
            case TYPE_NONE:
                list.emplace_back(Universe::None);
                break;
            case 't':
                str = this->get_string();
                list.emplace_back(str);
                this->m_string_table.emplace_back(str);
                break;
            case 's':
                list.emplace_back(this->get_string());
                break;
            case 'R': // ?
                list.emplace_back(this->m_string_table.at(this->read_int()));
                break;
            case '(':
                {
                    auto tuples = get_tuple();
                    auto ls = new List();
                    for (int i = 0; i < tuples->size(); ++i)
                    {
                        ls->append(ls->get(i));
                    }
                    list.emplace_back(ls);
                    break;
                };
            default:
                std::cout << "Parse tuple Error" << ", got unknown character " << obj_type << "\n";
                std::terminate();
            }
        }

        return new List(std::move(list));
    }

    List* BinaryFileReader::get_cell_vars()
    {
        return this->get_consts();
    }

    List* BinaryFileReader::get_free_vars()
    {
        return this->get_consts();
    }

    List* BinaryFileReader::get_var_names()
    {
        return this->get_consts();
    }

    List * BinaryFileReader::get_names()
    {
        return this->get_consts();
    }

    String* BinaryFileReader::get_no_table()
    {
        char ch = this->read();
        
        if (ch != 's' && ch != 't') 
        {
            this->unread();
            return nullptr;
        }
        
        return this->get_string();
    }

    String* BinaryFileReader::get_file_name()
    {
        return this->get_name();
    }

    String* BinaryFileReader::get_name()
    {
        char ch = this->read();

        switch (ch)
        {
        case 's': 
        {
            return this->get_string();
        }
        case 't': 
        {
            auto str = this->get_string();
            this->m_string_table.emplace_back(str);
            return str;
        }
        case 'R':
        {
            return this->m_string_table.at(this->read_int());
        }
        default: return nullptr;
        }

        // if (ch == 's') {
        //     return this->get_string();
        // }
        // else if (ch == 't') {
        //     String* str = this->get_string();
        //     this->m_string_table.emplace_back(str);
        //     return str;
        // }
        // else if (ch == 'R') {
        //     return this->m_string_table.at(this->read_int());
        // }

        // return nullptr;
    }

} // namespace python


