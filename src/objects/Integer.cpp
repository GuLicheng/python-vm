#include "Integer.hpp"
#include "Universe.hpp"
#include "Float.hpp"
#include "Operation.hpp"
#include "String.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "TypeObject.hpp"
#include "../Python.hpp"

namespace python
{
    Integer::Integer(int64_t value) : m_value(value)
    {
        this->set_klass(IntegerKlass::get_instance());
    }

    int64_t Integer::value() const
    {
        return this->m_value;
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
        {
            PYTHON_ASSERT(false && "Unsupported");
            return nullptr;
        }
    }

    void IntegerKlass::print(Object *x)
    {
        PYTHON_ASSERT(x && x->is<Integer>());
        std::cout << x->as<Integer>()->value();
    }

    Object* IntegerKlass::py__lt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less<>(), self, other);
    }

    Object* IntegerKlass::py__le__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::less_equal<>(), self, other);
    }

    Object* IntegerKlass::py__gt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater<>(), self, other);
    }

    Object* IntegerKlass::py__ge__(Object* self, Object* other)
    {
        return detail::binary_relation_operation_number(std::greater_equal<>(), self, other);
    }

    Object* IntegerKlass::py__eq__(Object* self, Object* other)
    {
        return detail::pyobject_equal<Integer>(self, other);
    }

    Object* IntegerKlass::py__ne__(Object* self, Object* other)
    {
        return detail::pyobject_not_equal<Integer>(self, other);
    }

    Object* IntegerKlass::py__add__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::plus<>(), self, other);
    }

    Object* IntegerKlass::py__sub__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::minus<>(), self, other);
    }

    Object* IntegerKlass::py__mul__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::multiplies<>(), self, other);
    }

    Object* IntegerKlass::py__div__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(std::divides<>(), self, other);
    }

    Object* IntegerKlass::py__mod__(Object* self, Object* other)
    {
        return detail::binary_arith_operation_number(detail::PythonModulo(), self, other);
    }

    Object* IntegerKlass::py__hash__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Integer>());
        auto cpp_int = self->as<Integer>()->value();
        return detail::cpp_hash_value2py_int(std::hash<int64_t>()(cpp_int));
    }

    Object* IntegerKlass::py__deepcopy__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Integer>());
        return new Integer(self->as<Integer>()->value());
    }

    Object* IntegerKlass::py__str__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<Integer>());
        return new String(std::to_string(self->as<Integer>()->value()));
    }
}
