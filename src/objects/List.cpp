#include "../Python.hpp"
#include "List.hpp"
#include "Dict.hpp"
#include "Universe.hpp"
#include "Integer.hpp"
#include "NativeFunction.hpp"
#include "FunctionObject.hpp"
#include "String.hpp"
#include "TypeObject.hpp"

#include <sstream>
#include <algorithm>




namespace python
{
	List::List() 
	{
		this->klass = ListKlass::get_instance();
	}

	List::List(int size)
	{
		this->inner_list.reserve(size);
		this->klass = ListKlass::get_instance();
	}

	List::List(std::vector<Object*> obj_list) : inner_list(std::move(obj_list))
	{
		this->klass = ListKlass::get_instance();
	}

	int List::size() const
	{
		return this->inner_list.size();
	}

	void List::append(Object* obj)
	{
		this->inner_list.emplace_back(obj);
	}

	Object* List::get(int index)
	{
		if (index < 0)
			index += this->size();
		PYTHON_ASSERT(index < this->size());
		return this->inner_list[index];
	}

	void List::set(int index, Object* o)
	{
		if (index < 0)
			index += this->size();
		PYTHON_ASSERT(index < this->size());
		this->inner_list[index] = o;
	}

	Object* List::top()
	{
		PYTHON_ASSERT(!this->inner_list.empty());
		return this->inner_list.back();
	}

    Object* List::pop()
    {
		PYTHON_ASSERT(this->inner_list.size() > 0);
        auto ret = this->inner_list.back();
		this->inner_list.pop_back();
		return ret; 
    }

    int List::index(Object* object)
    {
		auto location = std::ranges::find(this->inner_list, object);
		return location == this->inner_list.end() ?
				 -1 : std::ranges::distance(this->inner_list.begin(), location);
    }

    void List::delete_by_index(int index)
    {
		this->inner_list.erase(this->inner_list.begin() + index);
    }

    void List::delete_by_object(Object* obj)
    {
		auto idx = this->index(obj);
		if (idx != -1)
			this->delete_by_index(idx);
    }

    void List::reverse()
	{
		std::ranges::reverse(this->inner_list);
	}

    void List::insert(int pos, Object* obj)
    {
		this->inner_list.insert(this->inner_list.begin() + pos, obj);
    }

    ListKlass::ListKlass()
    {
	}

    void ListKlass::initialize()
    {
		Dict* dict = new Dict();
		dict->put(new String("append"), new FunctionObject(native::list_append));
		
		this->klass_dict = dict;

		(new TypeObject)->set_own_klass(this);
		this->set_name(new String("list"));
		this->add_super(ObjectKlass::get_instance());
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

    void ListKlass::print(Object *x)
    {
		List* lx = (List*)x;
		PYTHON_ASSERT(lx && lx->is<List>());

		std::cout << '[';

		int size = lx->size();
		if (size >= 1)
			lx->get(0)->print();

		for (int i = 1; i < size; i++) {
			printf(", ");
			lx->get(i)->print();
		}
		std::cout << ']';
	}

	Object* ListKlass::subscr(Object* x, Object* y)
	{
		PYTHON_ASSERT(x && x->is<List>());
		PYTHON_ASSERT(y && y->is<Integer>());

		auto index = y->as<Integer>()->value();
		return ((List*)x)->get(index);
	}

	Object* ListKlass::__contains__(Object* x, Object* y)
	{
		PYTHON_ASSERT(x && y && "x and y should not be nullptr.");

		List* ls = (List*)x;
		PYTHON_ASSERT(ls->is<List>());

		for (int i = 0; i < ls->size(); ++i)
		{
			if (ls->get(i)->__eq__(y))
				return Universe::True;
		}
		return Universe::False;
	}

	Object* ListKlass::__len__(Object* x)
	{
		PYTHON_ASSERT(x && x->is<List>());
		List* ls = x->as<List>();
		return new Integer(static_cast<int64_t>(ls->size()));
	}

    Object* ListKlass::__add__(Object* x, Object* y)
    {
        PYTHON_ASSERT(x && y && x->is<List>() && y->is<List>());
		
		auto ls1 = x->as<List>();
		auto ls2 = y->as<List>();

		auto result = std::vector<Object*>();
		result.reserve(ls1->size() + ls2->size());

		for (int i = 0; i < ls1->size(); ++i)
			result.emplace_back(ls1->get(i));
		
		for (int i = 0; i < ls2->size(); ++i)
			result.emplace_back(ls2->get(i));

		return new List(std::move(result));
    }


}