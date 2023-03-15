#include "Boolean.hpp"

namespace python
{
    Object* BooleanKlass::py__ror__(Object* self, Object* other)
    {
        return nullptr;
    }

    Boolean::Boolean(bool value) : m_value(value)
    {
        this->klass = BooleanKlass::get_instance();
    }

} // namespace python

