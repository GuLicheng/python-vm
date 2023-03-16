#include "Boolean.hpp"

namespace python
{
    Boolean::Boolean(bool value) : m_value(value)
    {
        this->set_klass(BooleanKlass::get_instance());
    }

} // namespace python

