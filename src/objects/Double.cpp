#include "Double.hpp"
#include "Operation.hpp"
#include "String.hpp"
#include "Dict.hpp"
#include "TypeObject.hpp"
#include "List.hpp"

#include <iostream>
#include <cmath>


namespace python
{
	Double::Double(double x) : val(x)
	{
		this->klass = DoubleKlass::get_instance();
	}

    void DoubleKlass::initialize()
    {
		this->set_klass_dict(new Dict());
		this->set_name(new String("double"));
		(new TypeObject)->set_own_klass(this);
		this->add_super(ObjectKlass::get_instance());
	}

    std::size_t DoubleKlass::size()
    {
        return sizeof(Double);
    }

    Object* DoubleKlass::allocate_instance(Object* callable, List* args)
    {
        if (!args || args->size() == 0)
			return new Double(0.0);
		return nullptr;
    }

    void DoubleKlass::print(Object *x)
    {
		PYTHON_ASSERT(x && x->is<Double>());
		std::cout << x->as<Double>()->value();
	}

	Object* DoubleKlass::__lt__(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::less<>(), x, y);
	}

	Object* DoubleKlass::__le__(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::less_equal<>(), x, y);
	}

	Object* DoubleKlass::__gt__(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::greater<>(), x, y);
	}

	Object* DoubleKlass::__ge__(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::greater_equal<>(), x, y);
	}

	Object* DoubleKlass::__eq__(Object* x, Object* y)
	{
		return detail::pyobject_equal<Double>(x, y);
	}

	Object* DoubleKlass::__ne__(Object* x, Object* y)
	{
		return detail::pyobject_not_equal<Double>(x, y);
	}

	Object* DoubleKlass::__add__(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::plus<>(), x, y);
	}

	Object* DoubleKlass::__sub__(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::minus<>(), x, y);
	}

	Object* DoubleKlass::__mul__(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::multiplies<>(), x, y);
	}

	Object* DoubleKlass::__div__(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::divides<>(), x, y);
	}

	// 2.5 % 3 = 2.5
	// 2.5 % 2 = 0.5
	// 2.5 % -3 = -0.5
	// 2.5 % -2 = -1.5
	Object* DoubleKlass::__mod__(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(detail::PythonModulo(), x, y);
	}

	Object* DoubleKlass::__hash__(Object* x)
	{
		PYTHON_ASSERT(x && x->is<Double>());
		auto cpp_double = x->as<Double>()->value();
		return detail::cpp_hash_value2py_int(std::hash<double>()(cpp_double));
	}

	Object* DoubleKlass::__deepcopy__(Object* x)
	{
		PYTHON_ASSERT(x && x->is<Double>());
		return new Double(x->as<Double>()->value());
	}

    Object* DoubleKlass::__str__(Object* x)
    {
		PYTHON_ASSERT(x && x->is<Double>());
        return new String(std::to_string(x->as<Double>()->value()));
    }
}

/*
=======================================
operator is  lt
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    0|    0|    0|    0|    0|
  -INF  |    1|    0|    0|    1|    1|
   NAN  |    0|    0|    0|    0|    0|
   POS  |    1|    0|    0|    0|    0|
   NEG  |    1|    0|    0|    1|    0|
=======================================
=======================================
operator is  le
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    1|    0|    0|    0|    0|
  -INF  |    1|    1|    0|    1|    1|
   NAN  |    0|    0|    0|    0|    0|
   POS  |    1|    0|    0|    1|    0|
   NEG  |    1|    0|    0|    1|    1|
=======================================
=======================================
operator is  eq
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    1|    0|    0|    0|    0|
  -INF  |    0|    1|    0|    0|    0|
   NAN  |    0|    0|    0|    0|    0|
   POS  |    0|    0|    0|    1|    0|
   NEG  |    0|    0|    0|    0|    1|
=======================================
=======================================
operator is  ne
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    0|    1|    1|    1|    1|
  -INF  |    1|    0|    1|    1|    1|
   NAN  |    1|    1|    1|    1|    1|
   POS  |    1|    1|    1|    0|    1|
   NEG  |    1|    1|    1|    1|    0|
=======================================
=======================================
operator is  gt
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    0|    1|    0|    1|    1|
  -INF  |    0|    0|    0|    0|    0|
   NAN  |    0|    0|    0|    0|    0|
   POS  |    0|    1|    0|    0|    1|
   NEG  |    0|    1|    0|    0|    0|
=======================================
=======================================
operator is  ge
=======================================
 py/cpp | +INF| -INF|  NAN|  POS|  NEG|
=======================================
  +INF  |    1|    1|    0|    1|    1|
  -INF  |    0|    1|    0|    0|    0|
   NAN  |    0|    0|    0|    0|    0|
   POS  |    0|    1|    0|    1|    1|
   NEG  |    0|    1|    0|    0|    1|
=======================================
*/