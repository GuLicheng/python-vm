#pragma once

#include "../objects/Object.hpp"
#include "../objects/Klass.hpp"
#include "../objects/Singleton.hpp"

namespace python
{

    class String;
    class List;
    class FrameObject;

    class StackElementKlass : public Klass, public Singleton<StackElementKlass>
    {
    public:
        
        StackElementKlass();
        
        virtual void print(Object* x) override;

        virtual std::size_t size() override;

    };

    class StackElement : public Object
    {
        friend class StackElementKlass;
    
        String* file_name;
    
        String* function_name;
    
        int line_no;

    public:

        using KlassType = StackElementKlass;

        StackElement(String* fname, String* mname, int lineno);

    };

    class TracebackKlass : public Klass, public Singleton<TracebackKlass>
    {
    public:

        TracebackKlass();

        virtual void print(Object* x) override;
    
        virtual std::size_t size() override;
    
    };

    class Traceback : public Object
    {
        friend class TracebackKlass;

        List* stack_elements;

    public:

        using KlassType = TracebackKlass;

        Traceback();

        void record_frame(FrameObject* frame);

    };

}
