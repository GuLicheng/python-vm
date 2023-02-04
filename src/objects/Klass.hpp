#pragma once

#include "../Python.hpp"

#define NOT_IMPLEMENT PYTHON_ASSERT(false && "not implement"); return 0 

namespace python
{


	class Object;
	class String;
	class Dict;
	class TypeObject;

	class Klass
	{
	protected:
		String* name = nullptr;
		
		Dict* klass_dict = nullptr;

		Klass* super = nullptr;

		TypeObject* type_object = nullptr;

		friend class Object;

	public:

		Klass() = default;

		void set_name(String* x) { this->name = x; }

		String* get_name() const { return this->name; }

		void set_klass_dict(Dict* d) { this->klass_dict = d; }

		Dict* get_klass_dict() { return this->klass_dict; }

		void set_type_object(TypeObject* obj) { this->type_object = obj; }

		TypeObject* get_type_object() { return this->type_object; }

		virtual void print(Object* obj) { PYTHON_ASSERT(false && "not implement"); }

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


		virtual Object* get_klass_attr(Object* x, Object* y) { NOT_IMPLEMENT; }

	};
}
