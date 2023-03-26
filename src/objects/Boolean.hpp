#pragma once

#include "Object.hpp"

namespace python
{
    class BooleanKlass : public Klass, public Singleton<BooleanKlass>
    {
    public:

        BooleanKlass();

        virtual Object* py__repr__(Object* self) override;

        // virtual Object* py__and__(Object* self, Object* other) override;

        // virtual Object* py__rand__(Object* self, Object* other) override;

        // virtual Object* py__xor__(Object* self, Object* other) override;

        // virtual Object* py__rxor__(Object* self, Object* other) override;

        // virtual Object* py__or__(Object* self, Object* other) override;
        
        // virtual Object* py__ror__(Object* self, Object* other) override;
        
        // virtual Object* py__new__(Object* self, Object* other) override;

        // virtual Object* py__doc__(Object* self, Object* other) override;

        // virtual void mark_self_and_children(Object* self);
    };

    class Boolean : public Object
    {
        friend class BooleanKlass;

        bool m_value;

    public:

        Boolean(bool value);

        using KlassType = BooleanKlass;
    };
}


/*
['__repr__',
 '__and__',
 '__rand__',
 '__xor__',
 '__rxor__',
 '__or__',
 '__ror__',
 '__new__',
 '__doc__']
*/