#pragma once

#include "Object.hpp"
#include "Double.hpp"
#include "Integer.hpp"
#include "../Python.hpp"
#include "Universe.hpp"

#include <functional>
#include <cmath>

namespace python::detail
{
    // This function requires x and y are same type
    template <typename ObjectType, typename Op>
    Object* binary_relation_operation(Op op, Object* x, Object* y)
    {
        using KlassT = typename ObjectType::KlassType;

        ObjectType* ix = (ObjectType*)x;
        ObjectType* iy = (ObjectType*)y;

        PYTHON_ASSERT(ix && (ix->get_klass() == KlassT::get_instance()) && "x should not be nullptr");
        PYTHON_ASSERT(iy && (iy->get_klass() == KlassT::get_instance()) && "y should not be nullptr");

        auto result = std::invoke(op, ix->value(), iy->value());
        return result ? Universe::True : Universe::False;
    }

    // This function requires x and y are same type
    template <typename ObjectType, typename Op>
    Object* binary_arith_operation(Op op, Object* x, Object* y)
    {
        using KlassT = typename ObjectType::KlassType;

        ObjectType* ix = (ObjectType*)x;
        ObjectType* iy = (ObjectType*)y;

        PYTHON_ASSERT(ix && (ix->get_klass() == KlassT::get_instance()) && "x should not be nullptr");
        PYTHON_ASSERT(iy && (iy->get_klass() == KlassT::get_instance()) && "y should not be nullptr");

        return new ObjectType(std::invoke(op, ix->value(), iy->value()));
    }

    inline bool is_number_object(Object* x)
    {
        PYTHON_ASSERT(x && "x should not be nullptr");
        return x->get_klass() == IntegerKlass::get_instance()
            || x->get_klass() == DoubleKlass::get_instance();
    }

    inline double convert2float(Object* x)
    {
        // assert x is not nullptr
        if (x->get_klass() == DoubleKlass::get_instance())
            return ((Double*)x)->value();
        else
            return ((Integer*)x)->value();
    }

    // This function requires both x and y must be one of (int/double)
    template <typename Op>
    Object* binary_relation_operation_number(Op op, Object* x, Object* y)
    {
        PYTHON_ASSERT(is_number_object(x) && is_number_object(y));

        // If x and y are same type, just call binary_relation_operation
        if (x->get_klass() == y->get_klass())
        {
            if (x->get_klass() == DoubleKlass::get_instance())
                return binary_relation_operation<Double>(op, x, y);
            else
                return binary_relation_operation<Integer>(op, x, y);
        }

        double dx = convert2float(x);
        double dy = convert2float(y);

        auto result = std::invoke(op, dx, dy);
        return result ? Universe::True : Universe::False;
    }

    // This function requires both x and y must be one of (int/double)
    template <typename Op>
    Object* binary_arith_operation_number(Op op, Object* x, Object* y)
    {
        PYTHON_ASSERT(is_number_object(x) && is_number_object(y));

        // If x and y are same type, just call binary_arith_operation
        if (x->get_klass() == y->get_klass())
        {
            if (x->get_klass() == DoubleKlass::get_instance())
                return binary_arith_operation<Double>(op, x, y);
            else
                return binary_arith_operation<Integer>(op, x, y);
        }

        double dx = convert2float(x);
        double dy = convert2float(y);

        return new Double(std::invoke(op, dx, dy));
    }

    template <typename ObjectType>
    Object* pyobject_equal(Object* x, Object* y)
    {
        PYTHON_ASSERT(x && y && "x and y should not be nullptr");

        if (x->get_klass() == y->get_klass())
            return binary_relation_operation<ObjectType>(std::equal_to<>(), x, y);

        // If x and y are not same type, but int/float type, convert
        // their value to double and compare. Otherwise return false.
        if (is_number_object(x) && is_number_object(y))
        {
            double dx = convert2float(x);
            double dy = convert2float(y);
            auto result = dx == dy;
            return result ? Universe::True : Universe::False;
        }
        else
        {
            return Universe::False;
        }

    }

    template <typename ObjectType>
    Object* pyobject_not_equal(Object* x, Object* y)
    {
        auto result = pyobject_equal<ObjectType>(x, y);

        return result == Universe::True ? Universe::False : Universe::True;
    }

    struct PythonModulo 
    {
        // https://github.com/python/cpython/blob/main/Objects/floatobject.c
        double operator()(double d1, double d2) const
        {
            double mod = std::fmod(d1, d2);
            if (mod) {
                /* ensure the remainder has the same sign as the denominator */
                if ((d2 < 0) != (mod < 0)) {
                    mod += d2;
                }
            }
            else {
                /* the remainder is zero, and in the presence of signed zeroes
                   fmod returns different results across platforms; ensure
                   it has the same sign as the denominator. */
                mod = std::copysign(0.0, d1);
            }
            return mod;
        }

        int64_t operator()(int64_t i1, int64_t i2) const
        {
            return i1 % i2;
        }

    };

    // hashcode
    inline Integer* cpp_hash_value2py_int(size_t hash_code)
    {
        return new Integer(static_cast<int64_t>(hash_code));
    }
}






