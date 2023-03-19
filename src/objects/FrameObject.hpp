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
        ArrayList<Object*>* m_stack;

        ArrayList<Block*>* m_loop_stack;

        ArrayList<Object*>* m_co_consts;

        ArrayList<Object*>* m_co_names;

        Dict* m_locals;

        Dict* m_globals;

        CodeObject* m_codes;

        List* m_fast_local; // save parameters 

        List* m_closure;

        FrameObject* m_sender;

        int m_pc;

        bool m_entry_frame;

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
