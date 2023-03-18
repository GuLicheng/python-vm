#pragma once

#include "CodeObject.hpp"
#include "../ArrayList.hpp"
#include "Object.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"

namespace python
{
    struct Block
    {
        unsigned char m_type;     // Block type, such as WHILE_BLOCK, EXCEPTION_BLOCK and so on.
        int m_target;             // Destination after executing the BREAK_LOOP
        int m_level;              // Block depth
    };

    class FrameObject
    {
        ArrayList<Object*>* stack;

        ArrayList<Block*>* loop_stack;

        ArrayList<Object*>* co_consts;

        ArrayList<Object*>* co_names;

        Dict* locals;

        Dict* globals;

        CodeObject* codes;

        List* fast_local; // save parameters 

        List* closure;

        FrameObject* sender;

        int pc;

        bool entry_frame;

        friend class Interpreter;

    public:

        FrameObject(CodeObject* codes);

        FrameObject(FunctionObject* function, List* args, int op_arg);

        bool has_more_codes() const;

        unsigned char get_op_code();

        int get_op_arg();

        bool is_first_frame() const;

        bool is_entry_frame() const;

        Object* get_cell_from_parameter(int index);

        int lineno();

        String* file_name();

        String* func_name();

    };
}
