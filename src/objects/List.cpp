#include "../Python.hpp"
#include "List.hpp"
#include "Dict.hpp"
#include "Universe.hpp"
#include "Integer.hpp"
#include "FunctionObject.hpp"
#include "String.hpp"
#include "TypeObject.hpp"
#include "Iterator.hpp"

#include <sstream>
#include <algorithm>
#include <iomanip>



namespace python
{
    List::List() 
    {
        this->set_klass(ListKlass::get_instance());
    }

    List::List(int size)
    {
        this->m_inner_list.reserve(size);
        this->set_klass(ListKlass::get_instance());
    }

    List::List(PythonList obj_list) : m_inner_list(std::move(obj_list))
    {
        this->set_klass(ListKlass::get_instance());
    }

    int List::size() const
    {
        return this->m_inner_list.size();
    }

    void List::append(Object* obj)
    {
        this->m_inner_list.emplace_back(obj);
    }

    Object* List::get(int index)
    {
        if (index < 0)
            index += this->size();
        PYTHON_ASSERT(index < this->size());
        return this->m_inner_list[index];
    }

    void List::set(int index, Object* o)
    {
        if (index < 0)
            index += this->size();
        PYTHON_ASSERT(index < this->size());
        this->m_inner_list[index] = o;
    }

    void List::fill_nullptr(int index)
    {
        if (index < this->size())
            return;
        this->m_inner_list.resize(index + 1);
    }

    Object* List::top()
    {
        PYTHON_ASSERT(!this->m_inner_list.empty());
        return this->m_inner_list.back();
    }

    Object* List::pop()
    {
        PYTHON_ASSERT(this->m_inner_list.size() > 0);
        auto ret = this->m_inner_list.back();
        this->m_inner_list.pop_back();
        return ret; 
    }

    int List::index(Object* object)
    {
        auto location = std::ranges::find(this->m_inner_list, object);
        return location == this->m_inner_list.end() ?
                 -1 : std::ranges::distance(this->m_inner_list.begin(), location);
    }

    void List::delete_by_index(int index)
    {
        this->m_inner_list.erase(this->m_inner_list.begin() + index);
    }

    void List::delete_by_object(Object* obj)
    {
        auto idx = this->index(obj);
        if (idx != -1)
            this->delete_by_index(idx);
    }

    void List::reverse()
    {
        std::ranges::reverse(this->m_inner_list);
    }

    void List::insert(int pos, Object* obj)
    {
        this->m_inner_list.insert(this->m_inner_list.begin() + pos, obj);
    }

    PythonList& List::value()
    {
        return this->m_inner_list;
    }

    void ListKlass::initialize()
    {
        Dict* dict = new Dict();
        
        dict->put(new String("append"), new FunctionObject(native::list_append));
        dict->put(new String("erase"), new FunctionObject(native::list_erase));
        dict->put(new String("pop"), new FunctionObject(native::list_pop));
        
        this->build_klass("list", ObjectKlass::get_instance(), dict);
    }

    Object* ListKlass::allocate_instance(Object* callable, List* args)
    {
        if (!args || args->size() == 0)
            return new List();
        else
            return nullptr;
    }

    size_t ListKlass::size()
    {
        return sizeof(List);
    }

    void ListKlass::print(Object *self)
    {
        List* lx = (List*)self;
        PYTHON_ASSERT(lx && lx->is<List>());

        std::cout << '[';

        int size = lx->size();

        for (int i = 0; i < size; i++) 
        {
            if (i != 0) std::cout << ", ";

            if (lx->get(i)->is<String>())    
                std::cout << std::quoted(lx->get(i)->as<String>()->value(), '"');
            else
                lx->get(i)->print();
        }
        std::cout << ']';
    }

    Object* ListKlass::py__getitem__(Object* self, Object* value)
    {
        PYTHON_ASSERT(self && self->expected<ListKlass>());
        PYTHON_ASSERT(value && value->expected<IntegerKlass>());

        auto index = value->as<Integer>()->value();
        return ((List*)self)->get(index);
    }

    void ListKlass::py__setitem__(Object* self, Object* key, Object* value)
    {
        auto idx = key->as<Integer>()->value();
        self->as<List>()->set(idx, value);
    }

    Object* ListKlass::py__contains__(Object* self, Object* value)
    {
        PYTHON_ASSERT(self && value && "self and value should not be nullptr.");

        List* ls = (List*)self;
        PYTHON_ASSERT(ls->is<List>());

        for (int i = 0; i < ls->size(); ++i)
        {
            if (ls->get(i)->py__eq__(value))
                return Universe::True;
        }
        return Universe::False;
    }

    Object* ListKlass::py__len__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<List>());
        List* ls = self->as<List>();
        return new Integer(static_cast<int64_t>(ls->size()));
    }

    Object* ListKlass::py__add__(Object* self, Object* other)
    {
        PYTHON_ASSERT(self && other && self->is<List>() && other->is<List>());
        
        auto ls1 = self->as<List>();
        auto ls2 = other->as<List>();

        auto result = PythonList();
        result.reserve(ls1->size() + ls2->size());

        for (int i = 0; i < ls1->size(); ++i)
            result.emplace_back(ls1->get(i));
        
        for (int i = 0; i < ls2->size(); ++i)
            result.emplace_back(ls2->get(i));

        return new List(std::move(result));
    }

    Object* ListKlass::py__iter__(Object* self)
    {
        return (new PyView(self, self->as<List>()->m_inner_list | std::views::all, "list_iterator"))->get_iterator();
    }

    Object* ListKlass::py__eq__(Object* self, Object* other)
    {
        if (self->get_klass() != other->get_klass())
            return Universe::False;

        if (self == other)
            return Universe::True;

        return std::ranges::equal(
            self->as<List>()->m_inner_list,
            other->as<List>()->m_inner_list,
            [](Object* lhs, Object* rhs) { 
                return lhs->py__eq__(rhs) == Universe::True; 
            }
        ) ? Universe::True : Universe::False;
    }

    Object* ListKlass::py__ne__(Object* self, Object* other)
    {
        return self->py__eq__(other) == Universe::True ? Universe::False : Universe::True;
    }

    Object* ListKlass::py__hash__(Object* self)
    {
        PYTHON_ASSERT(false && "List cannot hashable");
        return nullptr;
    }

}

namespace python::native
{
    Object* list_append(List* args) 
    {
        auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 2);
        arg0->as<List>()->append(args->get(1));
        return Universe::None;
    }
    
    Object* list_pop(List* args)
    {
        auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 1);
        return arg0->as<List>()->pop();
    }

    Object* list_remove(List* args)
    {
        auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 2);
        arg0->as<List>()->delete_by_object(args->get(1));
        return Universe::None;
    }

    Object* list_erase(List* args)
    {
        auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 2);
        auto index = args->get(1)->as<Integer>()->value();
        auto ret = arg0->as<List>()->get(index);
        arg0->as<List>()->delete_by_index(index);
        return ret;
    }
}