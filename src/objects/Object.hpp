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
        virtual void print(Object* self) override;
    };

    class Object 
    {
        friend class Klass;

    protected:
    
        Klass* m_klass = ObjectKlass::get_instance();

        Dict* m_obj_dict = nullptr;

        bool m_mark = false;

    public:

        using KlassType = ObjectKlass;

        Object() = default;

        Object(const Object&) = delete;

        void init_dict();

        void merge_obj_dict(Object* object);

        void mark();

        void mark_self_and_children();

        void unmark();

        bool is_marked();

        // virtual void show() { std::cout << "Object(None?)"; };

        Klass* get_klass()  
        { 
            PYTHON_ASSERT(this->m_klass); 
            return this->m_klass; 
        }

        void set_klass(Klass* k) { this->m_klass = k; }

        template <typename T>
        T* as() 
        {
            static_assert(std::is_base_of_v<Object, T>); 
            return static_cast<T*>(this); 
        }

        template <typename T>
        bool is() { return this->get_klass() == T::KlassType::get_instance(); }

        template <typename... TKlass>
        bool expected()
        {
            return ((this->m_klass == TKlass::get_instance()) || ...);
        }

        void print();

        // Operators
        // ==, !=, <, >, <=, >=
        Object* py__gt__(Object* self);
        Object* py__lt__(Object* self);
        Object* py__eq__(Object* self);
        Object* py__ne__(Object* self);
        Object* py__le__(Object* self);
        Object* py__ge__(Object* self);

        // +, -, *, /, //, %
        Object* py__add__(Object* self);
        Object* py__sub__(Object* self);
        Object* py__mul__(Object* self);
        Object* py__div__(Object* self);
        Object* py__floordiv__(Object* self);
        Object* py__mod__(Object* self);


        Object* py__contains__(Object* self);

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
