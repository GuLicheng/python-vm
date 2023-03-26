#pragma once

#include "Object.hpp"
#include "../ArrayList.hpp"
#include "String.hpp"

namespace python
{

    void show_code_object(class CodeObject* code, int level);

    class CodeObjectKlass : public Klass, public Singleton<CodeObjectKlass>
    {
    public:

        CodeObjectKlass();
    
        virtual void print(Object* obj) override;

        virtual void mark_self_and_children(Object* self) override;
    };

    struct CodeObject : public Object
    {
        int m_argcount;                    // arguments, except *args
        int m_nlocals;                     // local variables 
        int m_stacksize;                   // entries needed for evaluation stack
        int m_flags;                       // instruction opcodes

        String* m_code;

        // ArrayList<Object*>* co_consts;    // list (constants used)
        // ArrayList<Object*>* co_names;     // list of strings (names used)
        // ArrayList<Object*>* co_varnames;  // tuple of strings (local variable names)
        // ArrayList<Object*>* co_freevars;  // tuple of strings (free variable names)
        // ArrayList<Object*>* co_cellvars;  // tuple of strings (cell variable names)

        List* m_consts;    // list (constants used)
        List* m_names;     // list of strings (names used)
        List* m_varnames;  // tuple of strings (local variable names)
        List* m_freevars;  // tuple of strings (free variable names)
        List* m_cellvars;  // tuple of strings (cell variable names)

        /* The rest doesn't count for hash/cmp */
        String* m_filename;              // string (where it was loaded from) 
        String* m_name;                  // string (name, for reference)

        int m_firstlineno;                 // first source line number
        String* m_lnotab;

        // String* m_file_path;

        CodeObject();

        void print()  
        {
            show_code_object(this, 0);
        }
    };
}