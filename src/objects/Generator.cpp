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

    Object* GeneratorKlass::py__iter__(Object* self)
    {
        return self;
    }

    Object* GeneratorKlass::py__next__(Object* self)
    {
        PYTHON_ASSERT(self->is<Generator>());
        return Interpreter::get_instance()->eval_generator(self->as<Generator>());
    }

    std::size_t GeneratorKlass::size()
    {
        return sizeof(Generator);
    }

    void GeneratorKlass::mark_self_and_children(Object* self)
    {
        Generator* g = self->as<Generator>();
        if (g->is_marked())
            return;

        g->mark();
        g->m_frame->mark_self_and_children();
    }
    
    Generator::Generator(FunctionObject* func, List* args, int arg_cnt)
    {
        this->m_frame = new FrameObject(func, args, arg_cnt);
        this->set_klass(GeneratorKlass::get_instance());
    }

    void Generator::set_frame(FrameObject* frame)
    {
        this->m_frame = frame;
    }
}