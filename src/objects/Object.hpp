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
		virtual void print(Object* object) override;
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

		Object(const Object&) = delete;

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

		Object* __add__(Object* x);
		Object* __sub__(Object* x);
		Object* __mul__(Object* x);
		Object* __div__(Object* x);
		Object* __mod__(Object* x);

		Object* __gt__(Object* x);
		Object* __lt__(Object* x);
		Object* __eq__(Object* x);
		Object* __ne__(Object* x);
		Object* __le__(Object* x);
		Object* __ge__(Object* x);

		Object* subscr(Object* x);
		Object* __contains__(Object* x);

		Object* __hash__();
		Object* __deepcopy__();
		Object* __len__();
		Object* __str__();


		Object* __setattr__(Object* key, Object* value);
		Object* __getattr__(Object* attribute);
		Object* get_klass_attr(Object* attribute);

	};


}
