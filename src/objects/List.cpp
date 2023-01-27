#include "../Python.hpp"
#include "List.hpp"
#include "Universe.hpp"
#include "Integer.hpp"

#include <algorithm>

namespace python
{
	List::List() 
	{
		this->inner_list = new std::vector<Object*>();
		this->klass = ListKlass::get_instance();
	}

	List::List(int size)
	{
		this->inner_list = new std::vector<Object*>();
		this->inner_list->reserve(size);
		this->klass = ListKlass::get_instance();
	}

	List::List(std::vector<Object*>* obj_list) : inner_list(obj_list)
	{
		this->klass = ListKlass::get_instance();
	}

	int List::size() const
	{
		return this->inner_list->size();
	}

	void List::append(Object* obj)
	{
		this->inner_list->emplace_back(obj);
	}

	Object* List::get(int index)
	{
		if (index < 0)
			index += this->size();
		PYTHON_ASSERT(index < this->size());
		return this->inner_list->operator[](index);
	}

	void List::set(int index, Object* o)
	{
		if (index < 0)
			index += this->size();
		PYTHON_ASSERT(index < this->size());
		this->inner_list->operator[](index) = o;
	}

	Object* List::top()
	{
		PYTHON_ASSERT(!this->inner_list->empty());
		return this->inner_list->back();
	}

	void List::reverse()
	{
		PYTHON_ASSERT(this->inner_list != nullptr);
		std::ranges::reverse(*(this->inner_list));
	}

    void List::insert(int pos, Object* obj)
    {
		this->inner_list->insert(this->inner_list->begin() + pos, obj);
    }

    void ListKlass::print(Object* x)
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

	Object* ListKlass::contains(Object* x, Object* y)
	{
		PYTHON_ASSERT(x && y && "x and y should not be nullptr.");

		List* ls = (List*)x;
		PYTHON_ASSERT(ls->is<List>());

		for (int i = 0; i < ls->size(); ++i)
		{
			if (ls->get(i)->equal(y))
				return Universe::HiTrue;
		}
		return Universe::HiFalse;
	}

	Object* ListKlass::length(Object* x)
	{
		PYTHON_ASSERT(x && x->is<List>());
		List* ls = x->as<List>();
		return new Integer(static_cast<int64_t>(ls->size()));
	}

    Object* ListKlass::add(Object* x, Object* y)
    {
        PYTHON_ASSERT(x && y && x->is<List>() && y->is<List>());
		
		auto ls1 = x->as<List>();
		auto ls2 = y->as<List>();

		auto result = new std::vector<Object*>();
		result->reserve(ls1->size() + ls2->size());

		for (int i = 0; i < ls1->size(); ++i)
			result->emplace_back(ls1->get(i));
		
		for (int i = 0; i < ls2->size(); ++i)
			result->emplace_back(ls2->get(i));

		return new List(result);
    }
}