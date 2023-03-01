#include "Traceback.hpp"
#include "../objects/List.hpp"
#include "../objects/String.hpp"
#include "../objects/FrameObject.hpp"


namespace python
{
    void StackElementKlass::print(Object* x)
    {
        StackElement* xse = x->as<StackElement>();
        std::cout << "  File \"";
        xse->file_name->print();
        std::cout << "\" line " << xse->line_no;
        std::cout << " in ";
        xse->function_name->print();
        std::cout << '\n';
    }

    std::size_t StackElementKlass::size()
    {
        return sizeof(StackElement);
    }

    StackElement::StackElement(String* fname, String* mname, int lineno)
        : file_name(fname), function_name(mname), line_no(lineno)
    {
        this->set_klass(StackElementKlass::get_instance());
    }

    void TracebackKlass::print(Object* x)
    {
        auto tbx = x->as<Traceback>();
        std::cout << "Traceback (most recent call last):\n";
        for (int i = tbx->stack_elements->size() - 1; i >= 0; --i)
        {
            tbx->stack_elements->get(i)->print();
        }
    }

    std::size_t TracebackKlass::size()
    {
        return sizeof(Traceback);
    }

    Traceback::Traceback()
    {
        this->stack_elements = new List();
        this->set_klass(TracebackKlass::get_instance());
    }

    void Traceback::record_frame(FrameObject* frame)
    {
        this->stack_elements->append(new StackElement(
            frame->file_name(),
            frame->func_name(),
            frame->lineno()
        ));
    }

}