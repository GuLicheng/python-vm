#pragma once

#include "Object.hpp"

namespace python
{
    class Interpreter;
    class FrameObject;
    class List;
    class FunctionObject;

    class GeneratorKlass : public Klass, public Singleton<GeneratorKlass>
    {
    public:

        GeneratorKlass();

        virtual Object* __iter__(Object* x) override;

        virtual Object* __next__(Object* x) override;

        virtual std::size_t size() override;

    };  
    
    class Generator : public Object
    {
        friend class Interpreter;
        friend class FrameObject;
        friend class GeneratorKlass;

        FrameObject* frame;   // Save frame for next iteration

    public:

        using KlassType = GeneratorKlass;

        Generator(FunctionObject* func, List* args, int arg_cnt);

        void set_frame(FrameObject* f);

    };
}

