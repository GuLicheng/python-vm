#pragma once

#include "Klass.hpp"
#include "Singleton.hpp"
#include "../Python.hpp"

#include <type_traits>
#include <iostream>

namespace python
{

	class ObjectKlass : public Klass, public Singleton<ObjectKlass>
	{
	public:
		// virtual void print(Object* obj) override {
		// 	std::cout << "ObjectType\n";
		// }

	};

	class Object 
	{
		friend class Klass;

	protected:
	
		Klass* klass = ObjectKlass::get_instance();

		Dict* obj_dict = nullptr;

	public:

		using KlassType = ObjectKlass;

		Object() = default;

		//virtual ~Object() = default;

		void init_dict();

		virtual void show() { std::cout << "Object(None?)"; };

		Klass* get_klass()  
		{ 
			PYTHON_ASSERT(this->klass); 
			return this->klass; 
		}

		void set_klass(Klass* k) { this->klass = k; }

		template <typename T>
		T* as() 
		{
			static_assert(std::is_base_of_v<Object, T>); 
			return static_cast<T*>(this); 
		}

		template <typename T>
		bool is() { return this->get_klass() == T::KlassType::get_instance(); }


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


		Object* setattr(Object* key, Object* value);
		Object* getattr(Object* attribute);
		Object* get_klass_attr(Object* attribute);

	};


}
