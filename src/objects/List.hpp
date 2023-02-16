#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"

#include <span>
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

		virtual Object* subscr(Object* x, Object* y) override;
		
		virtual Object* contains(Object* x, Object* y) override;
		
		virtual Object* length(Object* x) override;

		virtual Object* add(Object* x, Object* y) override;

		// virtual Object* to_string(Object* x) override;
	};

	class List : public Object
	{
		std::vector<Object*> inner_list;

		friend class Interpreter;

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

		void delete_by_index(int index);

		void delete_by_object(Object* obj);

		void reverse();

		void insert(int pos, Object* obj);

	};


}

