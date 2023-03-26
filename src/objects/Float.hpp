#pragma once

#include "Object.hpp"
#include "Singleton.hpp"

namespace python
{
    class FloatKlass : public Klass, public Singleton<FloatKlass>
    {
    public:

        FloatKlass();

        void initialize();

        virtual std::size_t size() override;

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual void print(Object* self) override;

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

    class Float : public Object
    {
        double m_value;

    public:

        using KlassType = FloatKlass;

        Float(double value);

        double value() const;
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
 '__int__',
 '__float__',
 '__floordiv__',
 '__rfloordiv__',
 '__truediv__',
 '__rtruediv__',
 '__new__',
 '__trunc__',
 '__round__',
 '__getnewargs__',
 '__getformat__',
 '__set_format__',
 '__format__',
 '__doc__']
*/