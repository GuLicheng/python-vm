#include "FrameObject.hpp"
#include "Dict.hpp"

namespace python
{
    FrameObject::FrameObject(CodeObject* codes)
    {
        this->m_co_consts = codes->m_consts;
        this->m_co_names = codes->m_names;

        this->m_locals = new Dict();
        this->m_locals->put(new String("__name__"), new String("__main__"));

        this->m_globals = this->m_locals;

        this->m_stack = new List();
        // this->m_loop_stack = new ArrayList<Block*>();

        this->m_codes = codes;
        this->m_pc = 0;
        this->m_fast_local = nullptr;

        this->m_closure = nullptr;
        this->m_sender = nullptr;
        this->m_entry_frame = false;
        this->set_klass(FrameObjectKlass::get_instance());
    }

    FrameObject::FrameObject(FunctionObject* function, List* args, int op_arg) 
    {

        PYTHON_ASSERT((args && op_arg != 0) || (!args && op_arg == 0));

        this->m_co_consts = function->m_func_code->m_consts;
        this->m_co_names = function->m_func_code->m_names;

        this->m_locals = new Dict();
        this->m_globals = function->m_globals;

        this->m_stack = new List();
        // this->m_loop_stack = new ArrayList<Block*>();

        this->m_fast_local = new List();
        this->m_closure = 0;
        this->m_codes = function->m_func_code;
        this->m_pc = 0;
        this->set_klass(FrameObjectKlass::get_instance());
        

        /*
                    args:| 1 | 2 | 3 |
              defaults:| 1 | 2 | 4 | 5 | 6 |
              =>
            fast_local:| 1 | 2 | 3 | 5 | 6 |
        */
        
        const int na = op_arg & 0xFF;
        const int nk = op_arg >> 8;
        int argcnt = this->m_codes->m_argcount;
        int kw_pos = argcnt;


        this->m_fast_local->fill_nullptr(argcnt - 1);
        // for (int i = 0; i < argcnt; ++i)
        // {
        //     this->fast_local->append(nullptr);
        // }

        // Put default parameters to their position first.
        if (function->m_defaults)
        {
            int dft_cnt = function->m_defaults->size();
            while (dft_cnt--)
            {
                this->m_fast_local->set(--argcnt, function->m_defaults->get(dft_cnt));
            }
        }

        List* alist = nullptr;
        Dict* adict = nullptr;

        if (argcnt < na)
        {
            int i = 0;
            for (; i < argcnt; ++i)
            {
                this->m_fast_local->set(i, args->get(i));
            }
            alist = new List();
            for (; i < na; ++i)
            {
                alist->append(args->get(i));
            }
        }
        else
        {
            for (int i = 0; i < na; ++i)
            {
                this->m_fast_local->set(i, args->get(i));
            }
        }


        /*
            For such function:
            >>> def func(a, b, *args, **kwargs): pass
            >>> func(1, b = 0)
            For first argument `a`, the stack store it's value(1).
            For second argument `b`, the stack store it's name(b) and value(0).
            So we find the position by name(b) first and set value(0) to it's position
        */
        for (int i = 0; i < nk; ++i)
        {
            Object* key = args->get(na + i * 2);
            Object* val = args->get(na + i * 2 + 1);
            int index = this->m_codes->m_varnames->index(key);
            if (index >= 0)
            {
                this->m_fast_local->set(index, val);
            }
            else
            {
                if (!adict) 
                {
                    adict = new Dict();
                }
                adict->put(key, val);
            }
        }

        // If flag & CO_VARARGS == 1, the function has varargs
        if (this->m_codes->m_flags & FunctionObject::CO_VARARGS)
        {
            if (!alist)
            {
                alist = new List();
            }
            this->m_fast_local->fill_nullptr(argcnt); // CO_VARARGS may cause overflow
            this->m_fast_local->set(argcnt, alist);
            kw_pos++;
        }
        else
        {
            // Give more parameters than need
            if (alist)
            {
                PYTHON_ASSERT(false && "takes more extend parameters");
            }
        }

        // If flag & CO_VARKEYWORDS == 1, the function has kwargs
        if (this->m_codes->m_flags & FunctionObject::CO_VARKEYWORDS)
        {
            if (!adict)
            {
                adict = new Dict();
            }
            this->m_fast_local->set(kw_pos, adict);
        }
        else
        {
            // Give more parameters than need
            if (adict)
            {
                PYTHON_ASSERT(false && "takes more extend parameters");
            }
        }


        // Fill closure
        auto cells = this->m_codes->m_cellvars;
        if (cells && cells->size() > 0)
        {
            this->m_closure = new List();
            for (int i = 0; i < cells->size(); ++i)
            {
                this->m_closure->append(nullptr);
            }
        }

        if (function->m_closure && function->m_closure->size() > 0)
        {
            if (!this->m_closure)
            {
                this->m_closure = function->m_closure;
            }
            else
            {
                this->m_closure = (List*)this->m_closure->py__add__(function->m_closure);
            }
        }

        this->m_sender = nullptr;
        this->m_entry_frame = false;
    }

    bool FrameObject::has_more_codes() const
    {
        return this->m_pc < this->m_codes->m_code->length();
    }

    unsigned char FrameObject::get_op_code()
    {
        return this->m_codes->m_code->c_str()[this->m_pc++];
    }
    
    int FrameObject::get_op_arg()
    {
        auto co_code = this->m_codes->m_code->c_str();
        int byte1 = co_code[this->m_pc++] & 0xFF;
        int byte2 = co_code[this->m_pc++] & 0xFF;
        return (byte2 << 8) | byte1;
    }

    bool FrameObject::is_first_frame() const
    {
        return this->m_sender == nullptr;
    }

    bool FrameObject::is_entry_frame() const
    {
        return this->m_entry_frame;
    }

    Object* FrameObject::get_cell_from_parameter(int index)
    {
        Object* cell = this->m_codes->m_cellvars->get(index);
        index = this->m_codes->m_varnames->index(cell);
        return this->m_fast_local->get(index);
    }

    int FrameObject::lineno()
    {
        int pc_offset = 0;
        int line_no = this->m_codes->m_firstlineno;

        const char* lnotab = this->m_codes->m_lnotab->value().data();
        int length = this->m_codes->m_lnotab->length();

        for (int i = 0; i < length; i++) {
            pc_offset += lnotab[i++];
            if (pc_offset >= this->m_pc)
                return line_no;

            line_no += lnotab[i];
        }

        return line_no;
    }

    String* FrameObject::file_name()
    {
        return this->m_codes->m_filename;
    }

    String* FrameObject::func_name()
    {
        return this->m_codes->m_name;
    }
    
    FrameObjectKlass::FrameObjectKlass()
    {
        this->build_klass("frameobject", ObjectKlass::get_instance(), nullptr);
    }

    void FrameObjectKlass::mark_self_and_children(Object* self)
    {
        FrameObject* fo = self->as<FrameObject>();
        if (fo->is_marked())
            return;
        fo->mark();

        Klass::mark_all(
            fo->m_stack,
            fo->m_co_consts, 
            fo->m_co_names,
            fo->m_locals,
            fo->m_globals,
            fo->m_codes,
            fo->m_fast_local,
            fo->m_closure
        );
        if (fo->m_sender)
            fo->m_sender->mark_self_and_children();
    }
}
