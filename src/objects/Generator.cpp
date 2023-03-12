#include "Generator.hpp"
#include "List.hpp"
#include "FunctionObject.hpp"
#include "FrameObject.hpp"
#include "../runtime/Interpreter.hpp"

#include <iostream>

namespace python
{
    GeneratorKlass::GeneratorKlass()
    {
        this->build_klass("generator", ObjectKlass::get_instance(), nullptr);
    }

    Object* GeneratorKlass::py__iter__(Object* x)
    {
        return x;
    }

    Object* GeneratorKlass::py__next__(Object* x)
    {
        PYTHON_ASSERT(x->is<Generator>());
        return Interpreter::get_instance()->eval_generator(x->as<Generator>());
    }

    std::size_t GeneratorKlass::size()
    {
        return sizeof(Generator);
    }
    
    Generator::Generator(FunctionObject* func, List* args, int arg_cnt)
    {
        this->frame = new FrameObject(func, args, arg_cnt);
        this->set_klass(GeneratorKlass::get_instance());
    }

    void Generator::set_frame(FrameObject* f)
    {
        this->frame = f;
    }
}