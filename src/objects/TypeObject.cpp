#include "TypeObject.hpp"
#include "Dict.hpp"
#include "String.hpp"
#include "StringTable.hpp"
#include "Universe.hpp"

#include <iostream>

namespace python
{

    void TypeKlass::print(Object* x)
    {
        PYTHON_ASSERT(x->is<TypeObject>());       

        std::cout << "<type";

        Klass* own_klass = x->as<TypeObject>()->own_klass;

        Dict* attr_dict = own_klass->klass_dict;
        if (attr_dict)
        {
            Object* mod = attr_dict->get(StringTable::get_instance()->mod_str);
            if (mod != Universe::None)
            {
                mod->print();
                std::cout << ".";
            }
        }

        own_klass->name->print();
        std::cout << ">";

    }

    TypeObject::TypeObject()
    {
        this->klass = TypeKlass::get_instance();
    }

    void TypeObject::set_own_klass(Klass *k)
    {
        this->own_klass = k;
        k->set_type_object(this);
    }

    Klass *TypeObject::get_own_klass()
    {
        return this->klass;
    }

}