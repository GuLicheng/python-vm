#include "TypeObject.hpp"

namespace python
{
    TypeObject::TypeObject()
    {
        this->klass = TypeKlass::get_instance();
    }

    void TypeObject::set_own_klass(Klass *k)
    {
        this->klass = k;
    }

    Klass *TypeObject::get_own_klass()
    {
        return this->klass;
    }
}