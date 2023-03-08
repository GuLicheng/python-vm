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

        virtual Object* __lt__(Object* x, Object* y) override;
        virtual Object* __eq__(Object* x, Object* y) override;
        virtual Object* __gt__(Object* x, Object* y) override;
        virtual Object* __ne__(Object* x, Object* y) override;
        virtual Object* __le__(Object* x, Object* y) override;
        virtual Object* __ge__(Object* x, Object* y) override;

        virtual Object* __add__(Object* x, Object* y) override;
        virtual Object* __sub__(Object* x, Object* y) override;
        virtual Object* __mul__(Object* x, Object* y) override;
        virtual Object* __div__(Object* x, Object* y) override;
        virtual Object* __mod__(Object* x, Object* y) override;

        virtual Object* __hash__(Object* x) override;
        virtual Object* __deepcopy__(Object* x) override;
        virtual Object* __str__(Object* x) override;

    };

    class Integer : public Object
    {
        int64_t val;

    public:

        using KlassType = IntegerKlass;

        Integer(int64_t x);

        int64_t value() const { return this->val; }

        virtual void show() override { std::cout << val; }

    };

}
