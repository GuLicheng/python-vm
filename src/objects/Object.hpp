#pragma once

#include "Klass.hpp"
#include "Singleton.hpp"
#include "../Python.hpp"

#include <type_traits>
#include <iostream>

namespace python
{

    class ObjectKlass : public Klass, public Singleton<ObjectKlass>
    {
    public:
        virtual void print(Object* object) override;
    };

    class Object 
    {
        friend class Klass;

    protected:
    
        Klass* klass = ObjectKlass::get_instance();

        Dict* obj_dict = nullptr;

    public:

        using KlassType = ObjectKlass;

        Object() = default;

        Object(const Object&) = delete;

        void init_dict();

        virtual void show() { std::cout << "Object(None?)"; };

        Klass* get_klass()  
        { 
            PYTHON_ASSERT(this->klass); 
            return this->klass; 
        }

        void set_klass(Klass* k) { this->klass = k; }

        template <typename T>
        T* as() 
        {
            static_assert(std::is_base_of_v<Object, T>); 
            return static_cast<T*>(this); 
        }

        template <typename T>
        bool is() { return this->get_klass() == T::KlassType::get_instance(); }


        void print();

        // Operators
        // ==, !=, <, >, <=, >=
        Object* py__gt__(Object* x);
        Object* py__lt__(Object* x);
        Object* py__eq__(Object* x);
        Object* py__ne__(Object* x);
        Object* py__le__(Object* x);
        Object* py__ge__(Object* x);

        // +, -, *, /, //, %
        Object* py__add__(Object* x);
        Object* py__sub__(Object* x);
        Object* py__mul__(Object* x);
        Object* py__div__(Object* x);
        Object* py__floordiv__(Object* x);
        Object* py__mod__(Object* x);


        Object* py__contains__(Object* x);

        Object* py__hash__();
        Object* py__deepcopy__();
        Object* py__len__();
        Object* py__iter__();
        Object* py__next__();

        // Type convert str, int, float, bool, ...
        Object* py__str__();
        Object* py__int__();
        Object* py__float__();
        Object* py__bool__();


        Object* py__setattr__(Object* key, Object* value);
        Object* py__getattr__(Object* attribute);

        void py__setitem__(Object* key, Object* value);
        Object* py__getitem__(Object* name);

        Object* get_klass_attr(Object* attribute);

    };
}
