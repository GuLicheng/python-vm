#include "Integer.hpp"
#include "Universe.hpp"
#include "Double.hpp"
#include "Operation.hpp"
#include "String.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "TypeObject.hpp"
#include "../Python.hpp"

namespace python
{
	Integer::Integer(int64_t x) : val(x)
	{
		this->klass = IntegerKlass::get_instance();
	}

    void IntegerKlass::initialize()
    {
		this->set_klass_dict(new Dict());
		this->set_name(new String("int"));
		TypeObject().set_own_klass(this);
		this->add_super(ObjectKlass::get_instance());
    }

    size_t IntegerKlass::size()
    {
		return sizeof(Integer);
    }

    Object *IntegerKlass::allocate_instance(Object* callable, List* args)
    {
		if (!args || args->size() == 0)
			return new Integer(0);
		else
			return nullptr;
    }

    void IntegerKlass::print(Object *x)
    {
		PYTHON_ASSERT(x && x->is<Integer>());
		std::cout << x->as<Integer>()->value();
	}

	Object* IntegerKlass::less(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::less<>(), x, y);
	}

	Object* IntegerKlass::less_equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::less_equal<>(), x, y);
	}

	Object* IntegerKlass::greater(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::greater<>(), x, y);
	}

	Object* IntegerKlass::greater_equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation_number(std::greater_equal<>(), x, y);
	}

	Object* IntegerKlass::equal(Object* x, Object* y)
	{
		return detail::pyobject_equal<Integer>(x, y);
	}

	Object* IntegerKlass::not_equal(Object* x, Object* y)
	{
		return detail::pyobject_not_equal<Integer>(x, y);
	}

	Object* IntegerKlass::add(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::plus<>(), x, y);
	}

	Object* IntegerKlass::sub(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::minus<>(), x, y);
	}

	Object* IntegerKlass::mul(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::multiplies<>(), x, y);
	}

	Object* IntegerKlass::div(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(std::divides<>(), x, y);
	}

	Object* IntegerKlass::mod(Object* x, Object* y)
	{
		return detail::binary_arith_operation_number(detail::PythonModulo(), x, y);
	}

	Object* IntegerKlass::hash_code(Object* x)
	{
		PYTHON_ASSERT(x && x->is<Integer>());
		auto cpp_int = x->as<Integer>()->value();
		return detail::cpp_hash_value2py_int(std::hash<int64_t>()(cpp_int));
	}

	Object* IntegerKlass::deepcopy(Object* x)
	{
		PYTHON_ASSERT(x && x->is<Integer>());
		return new Integer(x->as<Integer>()->value());
	}

    Object* IntegerKlass::to_string(Object* x)
    {
		PYTHON_ASSERT(x && x->is<Integer>());
		return new String(std::to_string(x->as<Integer>()->value()));
    }
}
