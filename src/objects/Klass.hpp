#pragma once

#include "../Python.hpp"

#define NOT_IMPLEMENT PYTHON_ASSERT(false && "not implement"); return 0 

namespace python
{


	class Object;
	class String;
	class Dict;
	class TypeObject;
	class List;

	class Klass
	{

		friend class TypeKlass;

	protected:

		List* super = nullptr;  // super classes

		List* mro = nullptr;  // memory resolution order, control the process of methods searching

		TypeObject* type_object = nullptr;

		String* name = nullptr;
		
		Dict* klass_dict = nullptr;

		friend class Object;

		static Object* find_magic_method_and_call(Object* magic_method_name, Object* self);

		static Object* find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1);

		static Object* find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1, Object* arg2);

		static Object* find_and_call(Object* x, List* args, Object* function_name);

		static Object* find_in_parent(Object* x, Object* y);

		
	public:

		Klass() = default;

		Klass(const Klass&) = delete;

		static Object* create_klass(Object* x, Object* supers, Object* name);

		static int compare_klass(Klass* x, Klass* y);

		// Super
		void add_super(Klass* x);
		
		Object* get_super();
		
		void order_supers();

		void set_super_list(List* x) { this->super = x; }

		// Mro
		List* get_mro() { return this->mro; }

		// Default __init__
		virtual Object* allocate_instance(Object* callable, List* args);

		// Name
		void set_name(String* x) { this->name = x; }

		String* get_name() const { return this->name; }

		// Klass
		void set_klass_dict(Dict* d) { this->klass_dict = d; }

		Dict* get_klass_dict() { return this->klass_dict; }

		// Type
		void set_type_object(TypeObject* obj) { this->type_object = obj; }

		TypeObject* get_type_object() { return this->type_object; }

		// Member function
		virtual void print(Object* obj);


		// Magic methods

		// Operators
		// ==, !=, <, >, <=, >=
		virtual Object* __eq__(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* __ne__(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* __lt__(Object* x, Object* y);
		virtual Object* __gt__(Object* x, Object* y);
		virtual Object* __le__(Object* x, Object* y);
		virtual Object* __ge__(Object* x, Object* y);

		// +, -, *, /, //, %
		virtual Object* __add__(Object* x, Object* y);
		virtual Object* __sub__(Object* x, Object* y);
		virtual Object* __mul__(Object* x, Object* y);
		virtual Object* __div__(Object* x, Object* y) { NOT_IMPLEMENT; } //  operator /
		virtual Object* __floordiv__(Object* x, Object* y) { NOT_IMPLEMENT; } //   operator //
		virtual Object* __mod__(Object* x, Object* y) { NOT_IMPLEMENT; }
	
		virtual Object* __contains__(Object* x, Object* y) { NOT_IMPLEMENT; }

		// Type convert str, int, float, bool, ...
		virtual Object* __str__(Object* x) { NOT_IMPLEMENT; }
		virtual Object* __int__(Object* x) { NOT_IMPLEMENT; }
		virtual Object* __float__(Object* x) { NOT_IMPLEMENT; }
		virtual Object* __bool__(Object* x) { NOT_IMPLEMENT; }

		// Methods not implemented in book
		virtual Object* __hash__(Object* x);
		virtual Object* __deepcopy__(Object* x) { NOT_IMPLEMENT; }
		virtual Object* __len__(Object* x);


		virtual Object* get_klass_attr(Object* x, Object* y);

		virtual Object* __getattr__(Object* object, Object* attribute_name);
		virtual Object* __setattr__(Object* object, Object* key, Object* value);
		virtual Object* __getitem__(Object* object, Object* name);
		virtual void __setitem__(Object* object, Object* key, Object* value);

		// gc interface
		// virtual void oops_do(class Closure*, Object*);

		// class itself only
		virtual std::size_t size();
		// virtual void oops_do(class Closure*);

		// void* operator new(std::size_t);

	};
}
