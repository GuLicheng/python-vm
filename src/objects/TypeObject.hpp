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

        virtual void print(Object* x) override;

        virtual Object* setattr(Object* object, Object* key, Object* value) override;

    };

    class TypeObject : public Object
    {
        friend class TypeKlass;

        Klass* own_klass;

    public:

        using KlassType = TypeKlass;

        TypeObject();

        void set_own_klass(Klass* k);

        Klass* get_own_klass();

    };


}

