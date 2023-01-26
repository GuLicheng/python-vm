#pragma once

#include "Object.hpp"
#include "Singleton.hpp"

namespace python
{

	class DoubleKlass : public Klass, public Singleton<DoubleKlass>
	{
	public:

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
	
	};

	class Double : public Object
	{
		double val;

	public:

		using KlassType = DoubleKlass;

		Double(double x);

		double value() const { return this->val; }

		virtual void show() override
		{
			std::cout << val;
		}

	};


}
