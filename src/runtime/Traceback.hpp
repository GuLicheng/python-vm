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
        
        virtual void print(Object* self) override;

        virtual std::size_t size() override;
    };

    class StackElement : public Object
    {
        friend class StackElementKlass;
    
        String* m_file_name;
    
        String* m_function_name;
    
        int m_line_no;

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

        List* m_stack_elements;

    public:

        using KlassType = TracebackKlass;

        Traceback();

        void record_frame(FrameObject* frame);
    };
}
