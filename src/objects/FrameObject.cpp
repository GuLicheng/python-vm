#include "FrameObject.hpp"
#include "Dict.hpp"

namespace python
{
    FrameObject::FrameObject(CodeObject* codes)
    {
        this->co_consts = codes->co_consts;
        this->co_names = codes->co_names;

        this->locals = new Dict();
        this->locals->put(new String("__name__"), new String("__main__"));

        this->globals = this->locals;

        this->stack = new ArrayList<Object*>();
        this->loop_stack = new ArrayList<Block*>();

        this->codes = codes;
        this->pc = 0;
        this->fast_local = nullptr;

        this->closure = nullptr;
        this->sender = nullptr;
        this->entry_frame = false;
    }

    FrameObject::FrameObject(FunctionObject* function, List* args, int op_arg) 
    {

        PYTHON_ASSERT((args && op_arg != 0) || (!args && op_arg == 0));

        this->co_consts = function->func_code->co_consts;
        this->co_names = function->func_code->co_names;

        this->locals = new Dict();
        this->globals = function->globals;

        this->stack = new ArrayList<Object*>();
        this->loop_stack = new ArrayList<Block*>();

        this->fast_local = new List();
        this->closure = 0;
        this->codes = function->func_code;
        this->pc = 0;
        

        /*
                    args:| 1 | 2 | 3 |
              defaults:| 1 | 2 | 4 | 5 | 6 |
              =>
            fast_local:| 1 | 2 | 3 | 5 | 6 |
        */
        
        const int na = op_arg & 0xFF;
        const int nk = op_arg >> 8;
        int argcnt = this->codes->co_argcount;
        int kw_pos = argcnt;

        for (int i = 0; i < argcnt; ++i)
        {
            this->fast_local->append(nullptr);
        }

        // Put default parameters to their position first.
        if (function->defaults)
        {
            int dft_cnt = function->defaults->size();
            while (dft_cnt--)
            {
                this->fast_local->set(--argcnt, function->defaults->get(dft_cnt));
            }
        }

        List* alist = nullptr;
        Dict* adict = nullptr;

        if (argcnt < na)
        {
            int i = 0;
            for (; i < argcnt; ++i)
            {
                this->fast_local->set(i, args->get(i));
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
                this->fast_local->set(i, args->get(i));
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
            int index = this->codes->co_varnames->index(key);
            if (index >= 0)
            {
                this->fast_local->set(index, val);
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
        if (this->codes->co_flags & FunctionObject::CO_VARARGS)
        {
            if (!alist)
            {
                alist = new List();
            }
            this->fast_local->set(argcnt, alist);
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
        if (this->codes->co_flags & FunctionObject::CO_VARKEYWORDS)
        {
            if (!adict)
            {
                adict = new Dict();
            }
            this->fast_local->set(kw_pos, adict);
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
        auto cells = this->codes->co_cellvars;
        if (cells && cells->size() > 0)
        {
            this->closure = new List();
            for (int i = 0; i < cells->size(); ++i)
            {
                this->closure->append(nullptr);
            }
        }

        if (function->closure && function->closure->size() > 0)
        {
            if (!this->closure)
            {
                this->closure = function->closure;
            }
            else
            {
                this->closure = (List*)this->closure->__add__(function->closure);
            }
        }

        this->sender = nullptr;
        this->entry_frame = false;
    }

    bool FrameObject::has_more_codes() const
    {
        return this->pc < this->codes->co_code->length();
    }

    unsigned char FrameObject::get_op_code()
    {
        return this->codes->co_code->c_str()[this->pc++];
    }
    
    int FrameObject::get_op_arg()
    {
        auto co_code = this->codes->co_code->c_str();
        int byte1 = co_code[this->pc++] & 0xFF;
        int byte2 = co_code[this->pc++] & 0xFF;
        return (byte2 << 8) | byte1;
    }

    bool FrameObject::is_first_frame() const
    {
        return this->sender == nullptr;
    }

    bool FrameObject::is_entry_frame() const
    {
        return this->entry_frame;
    }

    Object* FrameObject::get_cell_from_parameter(int index)
    {
        Object* cell = this->codes->co_cellvars->get(index);
        index = this->codes->co_varnames->index(cell);
        return this->fast_local->get(index);
    }

    int FrameObject::lineno()
    {
        int pc_offset = 0;
        int line_no = this->codes->co_firstlineno;

        const char* lnotab = this->codes->co_lnotab->value().data();
        int length = this->codes->co_lnotab->length();

        for (int i = 0; i < length; i++) {
            pc_offset += lnotab[i++];
            if (pc_offset >= this->pc)
                return line_no;

            line_no += lnotab[i];
        }

        return line_no;
    }

    String* FrameObject::file_name()
    {
        return this->codes->co_filename;
    }

    String* FrameObject::func_name()
    {
        return this->codes->co_name;
    }


}
