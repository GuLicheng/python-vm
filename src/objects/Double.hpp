#pragma once

#include "Object.hpp"
#include "Singleton.hpp"

namespace python
{

    class DoubleKlass : public Klass, public Singleton<DoubleKlass>
    {
    public:

        DoubleKlass();

        void initialize();

        virtual std::size_t size() override;

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual void print(Object* x) override;

        virtual Object* py__lt__(Object* x, Object* y) override;
        virtual Object* py__eq__(Object* x, Object* y) override;
        virtual Object* py__gt__(Object* x, Object* y) override;
        virtual Object* py__ne__(Object* x, Object* y) override;
        virtual Object* py__le__(Object* x, Object* y) override;
        virtual Object* py__ge__(Object* x, Object* y) override;

        virtual Object* py__add__(Object* x, Object* y) override;
        virtual Object* py__sub__(Object* x, Object* y) override;
        virtual Object* py__mul__(Object* x, Object* y) override;
        virtual Object* py__div__(Object* x, Object* y) override;
        virtual Object* py__mod__(Object* x, Object* y) override;
    
        virtual Object* py__hash__(Object* x) override;
        virtual Object* py__deepcopy__(Object* x) override;
        virtual Object* py__str__(Object* x) override;
    
    };

    class Double : public Object
    {
        double val;

    public:

        using KlassType = DoubleKlass;

        Double(double x);

        double value() const { return this->val; }

        virtual void show() override
        {
            std::cout << val;
        }

    };


}
