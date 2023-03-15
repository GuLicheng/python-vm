#pragma once

#include "Object.hpp"
#include "Dict.hpp"
#include "String.hpp"
#include "NativeFunction.hpp"
#include "FunctionObject.hpp"

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
        Object* m_underlying;     // save list/dict to make it reachable for GC?

        TView m_view;

        std::string_view m_name;  // name for print

        PyView(Object* underlying, TView view, std::string_view name) 
            : m_underlying(underlying), m_view(std::move(view)), m_name(name) 
        {
            this->klass = PyViewKlass<TView>::get_instance();
        }

        struct PyViewIterator;

        struct PyViewIteratorKlass : public Klass, public Singleton<PyViewIteratorKlass>
        {
            PyViewIteratorKlass()
            {
                // auto builtin_magic = new Dict();
                // builtin_magic->put(new String("__iter__"), new FunctionObject(native::iter));
                // builtin_magic->put(new String("__next__"), new FunctionObject(native::next));
                // this->build_klass("PyViewIterator", ObjectKlass::get_instance(), builtin_magic);
                // Some magic methods should be register automatically.
                this->build_klass("PyViewIterator", ObjectKlass::get_instance(), nullptr);
            }

            virtual Object* py__iter__(Object* self) override
            {
                return self;
            }
            
            virtual Object* py__next__(Object* self) override
            {
                auto val = self->as<PyViewIterator>()->value();
                self->as<PyViewIterator>()->increase();
                return val;
            }

            virtual void print(Object* self) override
            {
                std::cout << self->as<PyViewIterator>()->m_parent->m_name;
            }
        };

        struct PyViewIterator : public Object
        {
            PyView* m_parent;

            std::ranges::iterator_t<TView> m_iterator;

            std::ranges::sentinel_t<TView> m_sentinel;

            PyViewIterator(PyView* p, std::ranges::iterator_t<TView> first, std::ranges::sentinel_t<TView> last) 
                : m_parent(p), m_iterator(std::move(first)), m_sentinel(std::move(last)) 
            {
                this->klass = PyViewIteratorKlass::get_instance();
            }

            void increase() 
            {
                // Some container may not support ++end() e.g. std::unordered_map 
                std::ranges::advance(this->m_iterator, 1, this->m_sentinel); 
            }

            bool is_over() const 
            { 
                return this->m_iterator == this->m_sentinel; 
            }

            Object* value() 
            { 
                return this->is_over() ? nullptr : *this->m_iterator; 
            }
        };

        Object* get_iterator()
        {
            return new PyViewIterator(this, std::ranges::begin(m_view), std::ranges::end(m_view));
        }
    };

    template <typename TView>
    struct PyViewKlass : public Klass, public Singleton<PyViewKlass<TView>>
    {
        PyViewKlass()
        {
            this->build_klass("PyViewKlass", ObjectKlass::get_instance(), nullptr);
        }

        virtual Object* py__iter__(Object* self) override
        {
            return self->as<PyView<TView>>()->get_iterator();
        }

        virtual void print(Object* self) override
        {
            std::cout << self->as<PyView<TView>>()->m_name;
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

    class ObjectView : public Object
    {
        Object* m_iterable;

        struct ObjectIterator
        {
            using value_type = Object*;
            using reference = Object*; 
            using difference_type = std::ptrdiff_t;
            using iterator_category = std::input_iterator_tag;

            ObjectView* m_parent;

            Object* m_iterator;

            Object* m_value; // store current value

            ObjectIterator() = default;
            
            ObjectIterator(std::default_sentinel_t) : ObjectIterator() { }

            ObjectIterator(ObjectView* parent) : m_parent(parent) 
            {
                this->m_iterator = this->m_parent->m_iterable->py__iter__();
                this->read();
            }

            ObjectIterator(const ObjectIterator&) = default;

            ObjectIterator& operator=(const ObjectIterator&) = default;

            reference operator*() const
            {
                return this->m_value;
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
                return this->m_iterator == nullptr || this->m_value == nullptr;
            }

            void read()
            {
                this->m_value = this->m_iterator->py__next__();
            }
        };

    public:

        ObjectView(Object* iterable) : m_iterable(iterable) 
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

