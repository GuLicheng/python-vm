#pragma once

#include "Object.hpp"
#include "Singleton.hpp"

namespace python
{

    class IntegerKlass : public Klass, public Singleton<IntegerKlass>
    {
    public:

        IntegerKlass();

        void initialize();

        virtual size_t size() override;

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual void print(Object* x) override;

        virtual Object* py__lt__(Object* self, Object* other) override;
        virtual Object* py__eq__(Object* self, Object* other) override;
        virtual Object* py__gt__(Object* self, Object* other) override;
        virtual Object* py__ne__(Object* self, Object* other) override;
        virtual Object* py__le__(Object* self, Object* other) override;
        virtual Object* py__ge__(Object* self, Object* other) override;

        virtual Object* py__add__(Object* self, Object* other) override;
        virtual Object* py__sub__(Object* self, Object* other) override;
        virtual Object* py__mul__(Object* self, Object* other) override;
        virtual Object* py__div__(Object* self, Object* other) override;
        virtual Object* py__mod__(Object* self, Object* other) override;

        virtual Object* py__hash__(Object* self) override;
        virtual Object* py__deepcopy__(Object* self) override;
        virtual Object* py__str__(Object* self) override;

        virtual void mark_self_and_children(Object* self) override;
    };

    class Integer : public Object
    {
        int64_t m_value;

    public:

        using KlassType = IntegerKlass;

        Integer(int64_t value);

        int64_t value() const;
    };
}
/*
['__repr__',
 '__hash__',
 '__getattribute__',
 '__lt__',
 '__le__',
 '__eq__',
 '__ne__',
 '__gt__',
 '__ge__',
 '__add__',
 '__radd__',
 '__sub__',
 '__rsub__',
 '__mul__',
 '__rmul__',
 '__mod__',
 '__rmod__',
 '__divmod__',
 '__rdivmod__',
 '__pow__',
 '__rpow__',
 '__neg__',
 '__pos__',
 '__abs__',
 '__bool__',
 '__invert__',
 '__lshift__',
 '__rlshift__',
 '__rshift__',
 '__rrshift__',
 '__and__',
 '__rand__',
 '__xor__',
 '__rxor__',
 '__or__',
 '__ror__',
 '__int__',
 '__float__',
 '__floordiv__',
 '__rfloordiv__',
 '__truediv__',
 '__rtruediv__',
 '__index__',
 '__new__',
 '__trunc__',
 '__floor__',
 '__ceil__',
 '__round__',
 '__getnewargs__',
 '__format__',
 '__sizeof__',
 '__doc__']

*/

