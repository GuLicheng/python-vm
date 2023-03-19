#include "FunctionObject.hpp"
#include "../Python.hpp"
#include "String.hpp"
#include "CodeObject.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "TypeObject.hpp"
#include "Object.hpp"

#include <iostream>
#include <functional>

namespace python
{
    FunctionKlass::FunctionKlass()
    {
        this->build_klass("function", ObjectKlass::get_instance(), new Dict());
    }

    void FunctionKlass::print(Object* self)
    {
        std::cout << "<Function: ";

        FunctionObject* fo = (FunctionObject*)self;

        // PYTHON_ASSERT(fo && fo->is<FunctionObject>());
        PYTHON_ASSERT(fo);

        fo->m_func_name->print();

        std::cout << ">";

    }
    
    FunctionObject::FunctionObject(Object* code_object)
    {
        CodeObject* co = (CodeObject*)code_object;

        this->m_func_code = co;
        this->m_func_name = co->co_name;
        this->m_flags = co->co_flags;
        this->m_globals = nullptr;
        this->m_native_func = nullptr;
        this->m_defaults = nullptr;
        this->m_closure = nullptr;

        this->set_klass(FunctionKlass::get_instance());
    }

    FunctionObject::FunctionObject(Klass* klass)
    {
        this->m_func_code = nullptr;
        this->m_func_name = nullptr;
        this->m_flags = 0;
        this->m_globals = nullptr;
        this->m_defaults = nullptr;
        this->m_native_func = nullptr;
        this->m_closure = nullptr;

        this->set_klass(klass);
    }

    FunctionObject::FunctionObject(NativeFunctionPointer nfp)
    {
        this->m_func_code = nullptr;
        this->m_func_name = nullptr;
        this->m_flags = 0;
        this->m_globals = nullptr;
        this->m_native_func = nfp;
        this->m_defaults = nullptr;
        this->m_closure = nullptr;

        this->set_klass(NativeFunctionKlass::get_instance());
    }

    Object* FunctionObject::call(List* args)
    {
        return std::invoke(this->m_native_func, args);
    }

    void FunctionObject::print()
    {
        std::cout << "FunctionObject";
        if (this->m_func_name)
            std::cout << " Name is " << this->m_func_name->value();
    }

    FunctionObject* FunctionObject::make_builtin_function(std::string_view fname, unsigned int fflag, List* default_params, NativeFunctionPointer ffunc)
    {
        auto f = new FunctionObject(ffunc);
        f->m_func_name = new String(fname.data(), fname.size());
        f->m_defaults = default_params;
        f->m_flags = fflag;
        return f;
    }

    MemberFunctionKlass::MemberFunctionKlass()
    {
        this->build_klass("method", FunctionKlass::get_instance(), new Dict());
    }

    void MemberFunctionKlass::print(Object* self)
    {
        std::cout << "MemberFunction";
    }

    MemberFunctionObject::MemberFunctionObject(FunctionObject* func) : m_owner(nullptr), m_func(func)
    {
        this->set_klass(MemberFunctionKlass::get_instance());
    }

    MemberFunctionObject::MemberFunctionObject(FunctionObject* func, Object* owner) : m_owner(owner), m_func(func)
    {
        this->set_klass(MemberFunctionKlass::get_instance());
    }

    bool MemberFunctionObject::is_function(Object* x)
    {
        Klass* k = x->get_klass();
        if (k == (Klass*) FunctionKlass::get_instance())
            return true;
        
        // if (!k->get_mro())
        //     return false;

        // for (int i = 0; i < k->get_mro()->size(); ++i)
        // {
        //     if (k->get_mro()->get(i) == FunctionKlass::get_instance()->get_type_object())
        //         return true;
        // }

        // return false;

        return x->get_klass()->contains_mro(FunctionKlass::get_instance());
    }

    bool MemberFunctionObject::is_yield_function(Object* x)
    {
        if (x->get_klass() != FunctionKlass::get_instance())
            return false;
        
        return (x->as<FunctionObject>()->m_flags & FunctionObject::CO_GENERATOR) != 0;
    }

    CellObject::CellObject(List *ls, int i)
    {
        this->m_table = ls;
        this->m_index = i;
        this->set_klass(CellKlass::get_instance());
    }

    Object* CellObject::value()
    {
        return this->m_table->get(this->m_index);
    }

    CellKlass::CellKlass()
    {
        this->build_klass("cell", ObjectKlass::get_instance(), nullptr);
    }

    void CellKlass::print(Object* self)
    {
        std::cout << "CellObject";
    }

    NativeFunctionKlass::NativeFunctionKlass()
    {
        this->build_klass("native_function", FunctionKlass::get_instance(), new Dict());
    }

    void NativeFunctionKlass::print(Object* self)
    {
        std::cout << "NativeFunctionObject";
        if (this->m_name)
            std::cout << " Name is " << this->m_name;
    }
}
