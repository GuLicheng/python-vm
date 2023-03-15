#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"
#include "Integer.hpp"
#include "Universe.hpp"
#include "List.hpp"

#include <unordered_map>

namespace python
{
    // Helper class 
    namespace detail
    {
        struct ObjectKeyEqual
        {
            using is_transparent = void;

            /* static */ size_t operator()(Object* x) const
            {
                PYTHON_ASSERT(x && "x should not be nullptr");
                auto py_int = x->py__hash__();
                auto cpp_int = py_int->as<Integer>()->value();
                return static_cast<size_t>(cpp_int);
            }

            bool operator()(Object* x, Object* y) const
            {
                auto py_boolean = x->py__eq__(y);
                return py_boolean == Universe::True ? true : false;
            }
        };
    }

    using PythonObjectDictionary = std::unordered_map<
        Object*,
        Object*,
        detail::ObjectKeyEqual,
        detail::ObjectKeyEqual
    >;


    class DictKlass : public Klass, public Singleton<DictKlass>
    {
    public:

        DictKlass() = default;

        void initialize();

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual size_t size() override;

        virtual Object* py__str__(Object* x) override;

        virtual Object* py__contains__(Object* x, Object* y) override;

        virtual Object* py__iter__(Object* x) override;

        virtual void print(Object* object) override;
        
    };

    class Dict : public Object
    {

        friend class DictKlass;

        PythonObjectDictionary dict;

    public:

        using KlassType = DictKlass;

        Dict();

        Dict(PythonObjectDictionary d);

        void put(Object* key, Object* value);

        Object* get(Object* key);

        bool has_key(Object* key);

        int size() const;

        Object* remove(Object* k);

        PythonObjectDictionary& value();

    };

}


namespace python::native
{
    // native function
    // An alternative way to implement iterator which is much simpler by using 
    // STL range adaptors
    // Is the garbage collection can work correct if we do not store the
    // reference of Object ?

    Object* dict_keys2(List* args);

    Object* dict_values2(List* args);

    Object* dict_items2(List* args);
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
 '__iter__',
 '__init__',
 '__len__',
 '__getitem__',
 '__setitem__',
 '__delitem__',
 '__contains__',
 '__new__',
 '__sizeof__',
 '__reversed__',
 '__doc__']
*/