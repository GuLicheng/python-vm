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
    Double::Double(double value) : m_value(value)
    {
        this->klass = DoubleKlass::get_instance();
    }

    double Double::value() const
    {
        return this->m_value;
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

    void DoubleKlass::print(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Double>());
        std::cout << self->as<Double>()->value();
    }

    Object* DoubleKlass::py__lt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less<>(), self, other);
    }

    Object* DoubleKlass::py__le__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less_equal<>(), self, other);
    }

    Object* DoubleKlass::py__gt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater<>(), self, other);
    }

    Object* DoubleKlass::py__ge__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater_equal<>(), self, other);
    }

    Object* DoubleKlass::py__eq__(Object* self, Object* other)
    {
        return detail::pyobject_equal<Double>(self, other);
    }

    Object* DoubleKlass::py__ne__(Object* self, Object* other)
    {
        return detail::pyobject_not_equal<Double>(self, other);
    }

    Object* DoubleKlass::py__add__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::plus<>(), self, other);
    }

    Object* DoubleKlass::py__sub__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::minus<>(), self, other);
    }

    Object* DoubleKlass::py__mul__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::multiplies<>(), self, other);
    }

    Object* DoubleKlass::py__div__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::divides<>(), self, other);
    }

    // 2.5 % 3 = 2.5
    // 2.5 % 2 = 0.5
    // 2.5 % -3 = -0.5
    // 2.5 % -2 = -1.5
    Object* DoubleKlass::py__mod__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(detail::PythonModulo(), self, other);
    }

    Object* DoubleKlass::py__hash__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Double>());
        auto cpp_double = self->as<Double>()->value();
        return detail::cpp_hash_value2py_int(std::hash<double>()(cpp_double));
    }

    Object* DoubleKlass::py__deepcopy__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Double>());
        return new Double(self->as<Double>()->value());
    }

    Object* DoubleKlass::py__str__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Double>());
        return new String(std::to_string(self->as<Double>()->value()));
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