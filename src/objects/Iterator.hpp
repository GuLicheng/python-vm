#pragma once

#include "Object.hpp"

#include <ranges>
#include <iterator>
#include <iostream>

namespace python
{
    template <typename TView>
    struct PyViewKlass;

    template <typename TView>
    struct PyView : public Object
    {
        Object* underlying;     // save list/dict to make it reachable for GC?

        TView view;

        std::string_view name;  // name for print

        PyView(Object* u, TView v, std::string_view n) 
            : underlying(u), view(std::move(v)), name(n) 
        {
            this->klass = PyViewKlass<TView>::get_instance();
        }

        struct PyViewIterator;

        struct PyViewIteratorKlass : public Klass, public Singleton<PyViewIteratorKlass>
        {
            PyViewIteratorKlass()
            {
                this->build_klass("PyViewIterator", ObjectKlass::get_instance(), nullptr);
            }

            virtual Object* py__iter__(Object* x) override
            {
                return x;
            }
            
            virtual Object* py__next__(Object* x) override
            {
                auto val = x->as<PyViewIterator>()->value();
                x->as<PyViewIterator>()->increase();
                return val;
            }

            virtual void print(Object* x) override
            {
                std::cout << x->as<PyViewIterator>()->parent->name;
            }
        };

        struct PyViewIterator : public Object
        {
            PyView* parent;

            std::ranges::iterator_t<TView> iterator;

            std::ranges::sentinel_t<TView> sentinel;

            PyViewIterator(PyView* p, std::ranges::iterator_t<TView> first, std::ranges::sentinel_t<TView> last) 
                : parent(p), iterator(std::move(first)), sentinel(std::move(last)) 
            {
                this->klass = PyViewIteratorKlass::get_instance();
            }

            void increase() 
            {
                // Some container may not support ++end() e.g. std::unordered_map 
                std::ranges::advance(this->iterator, 1, this->sentinel); 
            }

            bool is_over() const 
            { 
                return this->iterator == this->sentinel; 
            }

            Object* value() 
            { 
                return this->is_over() ? nullptr : *this->iterator; 
            }
        };

        Object* get_iterator()
        {
            return new PyViewIterator(this, std::ranges::begin(view), std::ranges::end(view));
        }
    };

    template <typename TView>
    struct PyViewKlass : public Klass, public Singleton<PyViewKlass<TView>>
    {
        PyViewKlass()
        {
            this->build_klass("PyViewKlass", ObjectKlass::get_instance(), nullptr);
        }

        virtual Object* py__iter__(Object* x) override
        {
            return x->as<PyView<TView>>()->get_iterator();
        }

        virtual void print(Object* x) override
        {
            std::cout << x->as<PyView<TView>>()->name;
        }
    };


    // Make Object become iterable

    class ObjectViewKlass : public Klass, public Singleton<ObjectViewKlass>
    {
    public:
        ObjectViewKlass() 
        {
            this->build_klass("ObjectViewKlass", ObjectKlass::get_instance(), nullptr);
        }
    };

    class ObjectView : public Object, public std::ranges::view_interface<ObjectView>
    {
        Object* iterable;

        struct ObjectIterator
        {
            using value_type = Object*;
            using reference = Object*; 
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

            ObjectView* parent;

            Object* iterator;

            Object* value; // store current value

            ObjectIterator() = default;
            
            ObjectIterator(std::default_sentinel_t) : ObjectIterator() { }

            ObjectIterator(ObjectView* parent) : parent(parent) 
            {
                this->iterator = this->parent->iterable->py__iter__();
                this->read();
            }

            ObjectIterator(const ObjectIterator&) = default;

            ObjectIterator& operator=(const ObjectIterator&) = default;

            reference operator*() const
            {
                return this->value;
            } 

            ObjectIterator& operator++()
            {
                if (!this->is_over())
                    this->read();
                return *this;
            }

            ObjectIterator operator++(int)
            {
                auto old = *this;
                ++*this;
                return old;
            }

            bool operator==(std::default_sentinel_t) const
            {
                return this->is_over();
            }

            bool is_over() const
            {
                return this->iterator == nullptr || this->value == nullptr;
            }

            void read()
            {
                this->value = this->iterator->py__next__();
            }

        };

    public:

        ObjectView(Object* iterable) : iterable(iterable) 
        {
            this->klass = ObjectKlass::get_instance();
        }

        ObjectIterator begin()
        {
            return ObjectIterator(this);
        }

        std::default_sentinel_t end()
        {
            return std::default_sentinel;
        }

    };


}

