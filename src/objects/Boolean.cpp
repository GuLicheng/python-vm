#include "Boolean.hpp"

namespace python
{
    Boolean::Boolean(bool value) : m_value(value)
    {
        this->klass = BooleanKlass::get_instance();
    }

} // namespace python

