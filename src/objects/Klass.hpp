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

		Object* find_and_call(Object* x, List* args, Object* function_name);
		
		Object* find_in_parent(Object* x, Object* y);


	public:

		Klass() = default;

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

		// Some relationship
		virtual Object* less(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* not_equal(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* equal(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* greater(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* less_equal(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* greater_equal(Object* x, Object* y) { NOT_IMPLEMENT; }

		// Some aritheric operation
		virtual Object* add(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* sub(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* mul(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* div(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* mod(Object* x, Object* y) { NOT_IMPLEMENT; }
	
		virtual Object* subscr(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* contains(Object* x, Object* y) { NOT_IMPLEMENT; }
		virtual Object* to_string(Object* x) { NOT_IMPLEMENT; }

		// Methods not implemented in book
		virtual Object* hash_code(Object* x) { NOT_IMPLEMENT; }
		virtual Object* deepcopy(Object* x) { NOT_IMPLEMENT; }
		virtual Object* length(Object* x) { NOT_IMPLEMENT; }


		virtual Object* get_klass_attr(Object* x, Object* y);

		virtual Object* getattr(Object* object, Object* attribute_name);
		virtual Object* setattr(Object* object, Object* key, Object* value);

		// gc interface
		// virtual void oops_do(class Closure*, Object*);

		// class itself only
		virtual std::size_t size();
		// virtual void oops_do(class Closure*);

		// void* operator new(std::size_t);

	};
}
