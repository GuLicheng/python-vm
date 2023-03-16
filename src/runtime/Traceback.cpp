#include "Traceback.hpp"
#include "../objects/List.hpp"
#include "../objects/String.hpp"
#include "../objects/FrameObject.hpp"

namespace python
{
    StackElementKlass::StackElementKlass()
    {
        this->build_klass("StackElement", ObjectKlass::get_instance(), nullptr);
    }

    void StackElementKlass::print(Object* self)
    {
        StackElement* xse = self->as<StackElement>();
        std::cout << "  File \"";
        xse->m_file_name->print();
        std::cout << "\" line " << xse->m_line_no;
        std::cout << " in ";
        xse->m_function_name->print();
        std::cout << '\n';
    }

    std::size_t StackElementKlass::size()
    {
        return sizeof(StackElement);
    }

    StackElement::StackElement(String* fname, String* mname, int lineno)
        : m_file_name(fname), m_function_name(mname), m_line_no(lineno)
    {
        this->set_klass(StackElementKlass::get_instance());
    }

    TracebackKlass::TracebackKlass()
    {
        this->build_klass("Traceback", ObjectKlass::get_instance(), nullptr);
    }

    void TracebackKlass::print(Object* self)
    {
        auto tbx = self->as<Traceback>();
        std::cout << "Traceback (most recent call last):\n";
        for (int i = tbx->m_stack_elements->size() - 1; i >= 0; --i)
        {
            tbx->m_stack_elements->get(i)->print();
        }
    }

    std::size_t TracebackKlass::size()
    {
        return sizeof(Traceback);
    }

    Traceback::Traceback()
    {
        this->m_stack_elements = new List();
        this->set_klass(TracebackKlass::get_instance());
    }

    void Traceback::record_frame(FrameObject* frame)
    {
        this->m_stack_elements->append(new StackElement(
            frame->file_name(),
            frame->func_name(),
            frame->lineno()
        ));
    }
}