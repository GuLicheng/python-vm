/*
    ---> | ListKlass  |
    |    | type_object| -----> |TypeObject|
    -------------------------- | own_klass|
                               |   klass  | -----> TypeKlass
*/

#pragma once

#include "Klass.hpp"
#include "Singleton.hpp"
#include "Object.hpp"

namespace python
{
    class TypeKlass : public Klass, public Singleton<TypeKlass>
    {
    public:

        // virtual void print(Object* x) override;

    };

    class TypeObject : public Object
    {

        Klass* own_klass;

    public:

        TypeObject();

        void set_own_klass(Klass* k);

        Klass* get_own_klass();

    };


}

