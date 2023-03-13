#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"

#include <array>
#include <vector>


namespace python
{
    class ListKlass : public Klass, public Singleton<ListKlass>
    {
    public:

        ListKlass() = default;

        void initialize();

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual size_t size() override;

        virtual void print(Object* x) override;

        virtual Object* py__getitem__(Object* x, Object* y) override;
        
        virtual Object* py__contains__(Object* x, Object* y) override;
        
        virtual Object* py__len__(Object* x) override;

        virtual Object* py__add__(Object* x, Object* y) override;

        virtual Object* py__iter__(Object* x) override;

        // virtual Object* py__str__(Object* x) override;
    };

    class List : public Object
    {
        std::vector<Object*> inner_list;

        friend class Interpreter;

        friend class ListIterator;

        friend class ListKlass;

    public:

        using KlassType = ListKlass;

        List();

        explicit List(int size);

        explicit List(std::vector<Object*> obj_list);

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

        // std::array<std::vector<Object*>::iterator, 2> get_iterator_pair();

        std::vector<Object*>& value();

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