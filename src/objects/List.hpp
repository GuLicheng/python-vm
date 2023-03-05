#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"
#include "Iterator.hpp"

#include <array>
#include <vector>


namespace python
{
	
	class ListKlass : public Klass, public Singleton<ListKlass>
	{
	public:

		ListKlass();

		void initialize();

    	virtual Object* allocate_instance(Object* callable, List* args) override;

		virtual size_t size() override;

		virtual void print(Object* x) override;

		virtual Object* __getitem__(Object* x, Object* y) override;
		
		virtual Object* __contains__(Object* x, Object* y) override;
		
		virtual Object* __len__(Object* x) override;

		virtual Object* __add__(Object* x, Object* y) override;

		virtual Object* __iter__(Object* x) override;

		// virtual Object* __str__(Object* x) override;
	};

	class List : public Object
	{
		std::vector<Object*> inner_list;

		friend class Interpreter;

		friend class ListIterator;

	public:

		using KlassType = ListKlass;

		List();

		explicit List(int size);

		explicit List(std::vector<Object*> obj_list);

		int size() const;

		void append(Object* obj);

		Object* get(int index);

		void set(int index, Object* o);

		Object* top();

		Object* pop();

		int index(Object* object);

		void delete_by_index(int index);

		void delete_by_object(Object* obj);

		void reverse();

		void insert(int pos, Object* obj);

		std::array<std::vector<Object*>::iterator, 2> get_iterator_pair();

	};

	struct ListIterator : public PyIterator<List, std::vector<Object*>::iterator, std::vector<Object*>::iterator>
	{

		using base = PyIterator<List, std::vector<Object*>::iterator>;
		
		ListIterator(List* list);

	};

	class ListIteratorKlass 
		: public PyIteratorKlass<ListIterator>, 
		  public Singleton<ListIteratorKlass>
	{
	public:

		ListIteratorKlass();

		void initialize();

		virtual void print(Object* x) override;

	};

}

