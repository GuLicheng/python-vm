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

    IntegerKlass::IntegerKlass()
    {
        this->initialize();
    }

    void IntegerKlass::initialize()
    {
        this->build_klass("int", ObjectKlass::get_instance(), nullptr);
    }

    size_t IntegerKlass::size()
    {
        return sizeof(Integer);
    }

    Object* IntegerKlass::allocate_instance(Object* callable, List* args)
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

    Object* IntegerKlass::py__lt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::less<>(), x, y);
    }

    Object* IntegerKlass::py__le__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::less_equal<>(), x, y);
    }

    Object* IntegerKlass::py__gt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::greater<>(), x, y);
    }

    Object* IntegerKlass::py__ge__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::greater_equal<>(), x, y);
    }

    Object* IntegerKlass::py__eq__(Object* x, Object* y)
    {
        return detail::pyobject_equal<Integer>(x, y);
    }

    Object* IntegerKlass::py__ne__(Object* x, Object* y)
    {
        return detail::pyobject_not_equal<Integer>(x, y);
    }

    Object* IntegerKlass::py__add__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::plus<>(), x, y);
    }

    Object* IntegerKlass::py__sub__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::minus<>(), x, y);
    }

    Object* IntegerKlass::py__mul__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::multiplies<>(), x, y);
    }

    Object* IntegerKlass::py__div__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::divides<>(), x, y);
    }

    Object* IntegerKlass::py__mod__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(detail::PythonModulo(), x, y);
    }

    Object* IntegerKlass::py__hash__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Integer>());
        auto cpp_int = x->as<Integer>()->value();
        return detail::cpp_hash_value2py_int(std::hash<int64_t>()(cpp_int));
    }

    Object* IntegerKlass::py__deepcopy__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Integer>());
        return new Integer(x->as<Integer>()->value());
    }

    Object* IntegerKlass::py__str__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Integer>());
        return new String(std::to_string(x->as<Integer>()->value()));
    }
}
