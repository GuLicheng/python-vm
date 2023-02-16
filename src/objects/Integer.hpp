#pragma once

#include "Object.hpp"
#include "Singleton.hpp"

namespace python
{

	class IntegerKlass : public Klass, public Singleton<IntegerKlass>
	{
	public:

		void initialize();

		virtual size_t size() override;

		virtual Object* allocate_instance(Object* callable, List* args) override;

		virtual void print(Object* x) override;

		virtual Object* less(Object* x, Object* y) override;
		virtual Object* equal(Object* x, Object* y) override;
		virtual Object* greater(Object* x, Object* y) override;
		virtual Object* not_equal(Object* x, Object* y) override;
		virtual Object* less_equal(Object* x, Object* y) override;
		virtual Object* greater_equal(Object* x, Object* y) override;

		virtual Object* add(Object* x, Object* y) override;
		virtual Object* sub(Object* x, Object* y) override;
		virtual Object* mul(Object* x, Object* y) override;
		virtual Object* div(Object* x, Object* y) override;
		virtual Object* mod(Object* x, Object* y) override;

		virtual Object* hash_code(Object* x) override;
		virtual Object* deepcopy(Object* x) override;
		virtual Object* to_string(Object* x) override;

	};

	class Integer : public Object
	{
		int64_t val;

	public:

		using KlassType = IntegerKlass;

		Integer(int64_t x);

		int64_t value() const { return this->val; }

		virtual void show() override { std::cout << val; }

	};

}
