#include "Boolean.hpp"
#include "String.hpp"
#include "Universe.hpp"
#include "Integer.hpp"

#include <functional>

namespace python
{
    Boolean::Boolean(bool value) : m_value(value)
    {
        this->set_klass(BooleanKlass::get_instance());
    }

    BooleanKlass::BooleanKlass()
    {
        this->build_klass("bool", ObjectKlass::get_instance(), nullptr);
    }

    Object* BooleanKlass::py__repr__(Object* self)
    {
        return new String(self->as<Boolean>()->m_value ? "True" : "False");
    }
} // namespace python

