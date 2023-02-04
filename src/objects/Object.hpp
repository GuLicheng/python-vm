#pragma once

#include "Klass.hpp"
#include "Singleton.hpp"
#include "../Python.hpp"

#include <iostream>

namespace python
{

	class ObjectKlass : public Klass, public Singleton<ObjectKlass>
	{

	};

	class Object 
	{
	protected:
	
		Klass* klass = ObjectKlass::get_instance();

	public:

		using KlassType = ObjectKlass;

		Object() = default;

		//virtual ~Object() = default;

		virtual void show() { std::cout << "Object(None?)"; };

		const Klass* get_klass() const 
		{ 
			PYTHON_ASSERT(this->klass); 
			return this->klass; 
		}

		void set_klass(Klass* k) { this->klass = k; }

		template <typename T>
		T* as() { return static_cast<T*>(this); }

		template <typename T>
		bool is() const { return this->get_klass() == T::KlassType::get_instance(); }


		void print();

		Object* add(Object* x);
		Object* sub(Object* x);
		Object* mul(Object* x);
		Object* div(Object* x);
		Object* mod(Object* x);

		Object* greater(Object* x);
		Object* less(Object* x);
		Object* equal(Object* x);
		Object* not_equal(Object* x);
		Object* less_equal(Object* x);
		Object* greater_equal(Object* x);

		Object* subscr(Object* x);
		Object* contains(Object* x);

		Object* hash_code();
		Object* deepcopy();
		Object* length();
		Object* to_string();


		Object* getattr(Object* x);

	};


}
