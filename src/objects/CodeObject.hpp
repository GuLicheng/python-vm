#pragma once

#include "Object.hpp"
#include "../ArrayList.hpp"
#include "String.hpp"

namespace python
{

    void show_code_object(class CodeObject* code, int level);

    struct CodeObject : public Object
    {
        int co_argcount;                    // arguments, except *args
        int co_nlocals;                     // local variables 
        int co_stacksize;                   // entries needed for evaluation stack
        int co_flags;                       // instruction opcodes

        String* co_code;

        ArrayList<Object*>* co_consts;    // list (constants used)
        ArrayList<Object*>* co_names;     // list of strings (names used)
        ArrayList<Object*>* co_varnames;  // tuple of strings (local variable names)
        ArrayList<Object*>* co_freevars;  // tuple of strings (free variable names)
        ArrayList<Object*>* co_cellvars;  // tuple of strings (cell variable names)

        /* The rest doesn't count for hash/cmp */
        String* co_filename;              // string (where it was loaded from) 
        String* co_name;                  // string (name, for reference)

        int co_firstlineno;                 // first source line number
        String* co_lnotab;

        String* file_path;

        CodeObject() = default;

        virtual void show() override 
        {
            show_code_object(this, 0);
        }
    };



}