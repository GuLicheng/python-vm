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

    void FunctionKlass::print(Object* obj)
    {
        std::cout << "<Function: ";

        FunctionObject* fo = (FunctionObject*)obj;

        // PYTHON_ASSERT(fo && fo->is<FunctionObject>());
        PYTHON_ASSERT(fo);

        fo->func_name->print();

        std::cout << ">";

    }
    
    FunctionObject::FunctionObject(Object* code_object)
    {
        CodeObject* co = (CodeObject*)code_object;

        this->func_code = co;
        this->func_name = co->co_name;
        this->flags = co->co_flags;
        this->globals = nullptr;
        this->native_func = nullptr;
        this->defaults = nullptr;
        this->closure = nullptr;

        this->klass = FunctionKlass::get_instance();
    }

    FunctionObject::FunctionObject(Klass* klass)
    {
        this->func_code = nullptr;
        this->func_name = nullptr;
        this->flags = 0;
        this->globals = nullptr;
        this->defaults = nullptr;
        this->native_func = nullptr;
        this->closure = nullptr;

        this->klass = klass;
    }

    FunctionObject::FunctionObject(NativeFunctionPointer nfp)
    {
        this->func_code = nullptr;
        this->func_name = nullptr;
        this->flags = 0;
        this->globals = nullptr;
        this->native_func = nfp;
        this->defaults = nullptr;
        this->closure = nullptr;

        this->klass = NativeFunctionKlass::get_instance();
    }

    Object* FunctionObject::call(List* args)
    {
        return std::invoke(this->native_func, args);
    }

    void FunctionObject::print()
    {
        std::cout << "FunctionObject";
        if (this->func_name)
            std::cout << " Name is " << this->func_name->value();
    }

    MemberFunctionKlass::MemberFunctionKlass()
    {
        this->build_klass("method", FunctionKlass::get_instance(), new Dict());
    }

    void MemberFunctionKlass::print(Object* object)
    {
        std::cout << "MemberFunction";
    }

    MemberFunctionObject::MemberFunctionObject(FunctionObject* func) : owner(nullptr), func(func)
    {
        this->klass = MemberFunctionKlass::get_instance();
    }

    MemberFunctionObject::MemberFunctionObject(FunctionObject* func, Object* owner) : owner(owner), func(func)
    {
        this->klass = MemberFunctionKlass::get_instance();
    }

    bool MemberFunctionObject::is_function(Object* x)
    {
        Klass* k = x->get_klass();
        if (k == (Klass*) FunctionKlass::get_instance())
            return true;
        
        if (!k->get_mro())
            return false;

        for (int i = 0; i < k->get_mro()->size(); ++i)
        {
            if (k->get_mro()->get(i) == FunctionKlass::get_instance()->get_type_object())
                return true;
        }

        return false;
    }

    bool MemberFunctionObject::is_yield_function(Object* x)
    {
        if (x->get_klass() != FunctionKlass::get_instance())
            return false;
        
        return (x->as<FunctionObject>()->flags & FunctionObject::CO_GENERATOR) != 0;
    }

    CellObject::CellObject(List *ls, int i)
    {
        this->table = ls;
        this->index = i;
        this->klass = CellKlass::get_instance();
    }

    Object *CellObject::value()
    {
        return this->table->get(this->index);
    }

    CellKlass::CellKlass()
    {
        this->build_klass("cell", ObjectKlass::get_instance(), nullptr);
    }

    void CellKlass::print(Object* object)
    {
        std::cout << "CellObject";
    }

    NativeFunctionKlass::NativeFunctionKlass()
    {
        this->build_klass("native_function", FunctionKlass::get_instance(), new Dict());
    }

    void NativeFunctionKlass::print(Object* object)
    {
        std::cout << "NativeFunctionObject";
        if (this->name)
            std::cout << " Name is " << this->name;
    }

}
