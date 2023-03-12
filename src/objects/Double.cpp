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

    DoubleKlass::DoubleKlass()
    {
        this->initialize();
    }

    void DoubleKlass::initialize()
    {
        this->build_klass("float", ObjectKlass::get_instance(), new Dict());
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

    Object* DoubleKlass::py__lt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::less<>(), x, y);
    }

    Object* DoubleKlass::py__le__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::less_equal<>(), x, y);
    }

    Object* DoubleKlass::py__gt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::greater<>(), x, y);
    }

    Object* DoubleKlass::py__ge__(Object* x, Object* y)
    {
        return detail::binary_relation_operation_number(std::greater_equal<>(), x, y);
    }

    Object* DoubleKlass::py__eq__(Object* x, Object* y)
    {
        return detail::pyobject_equal<Double>(x, y);
    }

    Object* DoubleKlass::py__ne__(Object* x, Object* y)
    {
        return detail::pyobject_not_equal<Double>(x, y);
    }

    Object* DoubleKlass::py__add__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::plus<>(), x, y);
    }

    Object* DoubleKlass::py__sub__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::minus<>(), x, y);
    }

    Object* DoubleKlass::py__mul__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::multiplies<>(), x, y);
    }

    Object* DoubleKlass::py__div__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(std::divides<>(), x, y);
    }

    // 2.5 % 3 = 2.5
    // 2.5 % 2 = 0.5
    // 2.5 % -3 = -0.5
    // 2.5 % -2 = -1.5
    Object* DoubleKlass::py__mod__(Object* x, Object* y)
    {
        return detail::binary_arith_operation_number(detail::PythonModulo(), x, y);
    }

    Object* DoubleKlass::py__hash__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Double>());
        auto cpp_double = x->as<Double>()->value();
        return detail::cpp_hash_value2py_int(std::hash<double>()(cpp_double));
    }

    Object* DoubleKlass::py__deepcopy__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Double>());
        return new Double(x->as<Double>()->value());
    }

    Object* DoubleKlass::py__str__(Object* x)
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