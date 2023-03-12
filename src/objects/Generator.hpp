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

        virtual Object* py__iter__(Object* x) override;

        virtual Object* py__next__(Object* x) override;

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

