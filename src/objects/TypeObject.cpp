#include "TypeObject.hpp"
#include "Dict.hpp"
#include "String.hpp"
#include "StringTable.hpp"
#include "Universe.hpp"

#include <iostream>

namespace python
{
    void TypeKlass::print(Object* self)
    {
        PYTHON_ASSERT(self->is<TypeObject>());

        std::cout << "<type";

        Klass* own_klass = self->as<TypeObject>()->m_own_klass;

        Dict* attr_dict = own_klass->klass_dict;
        if (attr_dict)
        {
            Object* mod = attr_dict->get(StringTable::module);
            if (mod != Universe::None)
            {
                mod->print();
                std::cout << ".";
            }
        }

        own_klass->name->print();
        std::cout << ">";

    }

    Object* TypeKlass::py__setattr__(Object* self, Object* key, Object* value)
    {
        // Add attribute into klass dict
        self->as<TypeObject>()->get_own_klass()->get_klass_dict()->put(key, value);
        return Universe::None;
    }

    TypeObject::TypeObject()
    {
        this->set_klass(TypeKlass::get_instance());
    }

    void TypeObject::set_own_klass(Klass* klass)
    {
        this->m_own_klass = klass;
        klass->set_type_object(this);
    }

    Klass* TypeObject::get_own_klass()
    {
        return this->m_own_klass;
    }
}