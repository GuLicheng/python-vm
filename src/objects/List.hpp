#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"

#include <array>
#include <vector>

namespace python
{
    using PythonList = std::vector<Object*>;

    class ListKlass : public Klass, public Singleton<ListKlass>
    {
    public:

        ListKlass() = default;

        void initialize();

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual size_t size() override;

        virtual void print(Object* self) override;

        virtual Object* py__getitem__(Object* self, Object* value) override;

        virtual void py__setitem__(Object* object, Object* key, Object* value);
        
        virtual Object* py__contains__(Object* self, Object* value) override;
        
        virtual Object* py__len__(Object* self) override;

        virtual Object* py__add__(Object* self, Object* other) override;

        virtual Object* py__iter__(Object* self) override;

        virtual Object* py__eq__(Object* self, Object* other) override;

        virtual Object* py__ne__(Object* self, Object* other) override;

        virtual Object* py__hash__(Object* self) override;

        // virtual Object* py__str__(Object* self) override;
    };

    class List : public Object
    {
        PythonList m_inner_list;

        friend class Interpreter;

        friend class ListIterator;

        friend class ListKlass;

    public:

        using KlassType = ListKlass;

        List();

        explicit List(int size);

        explicit List(PythonList obj_list);

        int size() const;

        void append(Object* obj);

        Object* get(int index);

        void set(int index, Object* o);

        void fill_nullptr(int index);

        Object* top();

        Object* pop();

        int index(Object* object);

        void delete_by_index(int index);

        void delete_by_object(Object* obj);

        void reverse();

        void insert(int pos, Object* obj);

        PythonList& value();
    };
}

namespace python::native::detail
{
    template <typename T>
    Object* check_and_get_from_argument_list(List* args, int index, int argc)
    {
        PYTHON_ASSERT(args && args->size() == argc && "args should be available");
        auto arg = args->get(index);
        PYTHON_ASSERT(arg->is<T>());
        return arg;
    }

    // If argc > 0, then the size of argument-list must be equal to argc.
    // Otherwise, there are at least argc arguments in argument-list.
    inline void check_argument_count(List* args, int argc)
    {
        if (argc > 0)
            PYTHON_ASSERT(args && args->size() == argc);
        else
            PYTHON_ASSERT(args && args->size() >= -argc);
    }
}

namespace python::native
{
    Object* list_append(List* args);

    Object* list_pop(List* args);

    Object* list_remove(List* args);

    Object* list_erase(List* args);
    
    // Object* list_reverse(List* args);
    // Object* list_sort(List* args);
    // Object* list_extend(List* args);
    // Object* list_index(List* args);
    // Object* list_getitem(List* args);
}

/*
[
    '__repr__',
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
    '__add__',
    '__mul__',
    '__rmul__',
    '__contains__',
    '__iadd__',
    '__imul__',
    '__new__',
    '__reversed__',
    '__sizeof__',
    '__doc__'
]
*/