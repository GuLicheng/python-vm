#include "TypeObject.hpp"

namespace python
{
    TypeObject::TypeObject()
    {
        this->set_klass(TypeKlass::get_instance());
    }

}