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

        virtual void print(Object* self) override;

        virtual Object* py__setattr__(Object* self, Object* key, Object* value) override;

    };

    class TypeObject : public Object
    {
        friend class TypeKlass;

        Klass* m_own_klass;

    public:

        using KlassType = TypeKlass;

        TypeObject();

        void set_own_klass(Klass* klass);

        Klass* get_own_klass();

    };


}

