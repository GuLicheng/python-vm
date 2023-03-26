#include "Float.hpp"
#include "Operation.hpp"
#include "String.hpp"
#include "Dict.hpp"
#include "TypeObject.hpp"
#include "List.hpp"

#include <iostream>
#include <cmath>


namespace python
{
    Float::Float(double value) : m_value(value)
    {
        this->set_klass(FloatKlass::get_instance());
    }

    double Float::value() const
    {
        return this->m_value;
    }

    FloatKlass::FloatKlass()
    {
        this->initialize();
    }

    void FloatKlass::initialize()
    {
        this->build_klass("float", ObjectKlass::get_instance(), new Dict());
    }

    std::size_t FloatKlass::size()
    {
        return sizeof(Float);
    }

    Object* FloatKlass::allocate_instance(Object* callable, List* args)
    {
        if (!args || args->size() == 0)
            return new Float(0.0);
        return nullptr;
    }

    void FloatKlass::print(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Float>());
        std::cout << self->as<Float>()->value();
    }

    Object* FloatKlass::py__lt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less<>(), self, other);
    }

    Object* FloatKlass::py__le__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less_equal<>(), self, other);
    }

    Object* FloatKlass::py__gt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater<>(), self, other);
    }

    Object* FloatKlass::py__ge__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater_equal<>(), self, other);
    }

    Object* FloatKlass::py__eq__(Object* self, Object* other)
    {
        return detail::pyobject_equal<Float>(self, other);
    }

    Object* FloatKlass::py__ne__(Object* self, Object* other)
    {
        return detail::pyobject_not_equal<Float>(self, other);
    }

    Object* FloatKlass::py__add__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::plus<>(), self, other);
    }

    Object* FloatKlass::py__sub__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::minus<>(), self, other);
    }

    Object* FloatKlass::py__mul__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::multiplies<>(), self, other);
    }

    Object* FloatKlass::py__div__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::divides<>(), self, other);
    }

    // 2.5 % 3 = 2.5
    // 2.5 % 2 = 0.5
    // 2.5 % -3 = -0.5
    // 2.5 % -2 = -1.5
    Object* FloatKlass::py__mod__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(detail::PythonModulo(), self, other);
    }

    Object* FloatKlass::py__hash__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Float>());
        auto cpp_double = self->as<Float>()->value();
        return detail::cpp_hash_value2py_int(std::hash<double>()(cpp_double));
    }

    Object* FloatKlass::py__deepcopy__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Float>());
        return new Float(self->as<Float>()->value());
    }

    Object* FloatKlass::py__str__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Float>());
        return new String(std::to_string(self->as<Float>()->value()));
    }

    void FloatKlass::mark_self_and_children(Object* self)
    {
        self->mark();
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