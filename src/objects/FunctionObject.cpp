#include "FunctionObject.hpp"
#include "../Python.hpp"
#include "String.hpp"
#include "CodeObject.hpp"
#include "Dict.hpp"

#include <iostream>
#include <functional>

namespace python
{
	void FunctionKlass::print(Object* obj)
	{
		std::cout << "<Function: ";

		FunctionObject* fo = (FunctionObject*)obj;

		PYTHON_ASSERT(fo && fo->is<FunctionObject>());

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

		this->set_klass(FunctionKlass::get_instance());
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

		this->set_klass(klass);
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

		this->set_klass(NativeFunctionKlass::get_instance());
	}

	Object* FunctionObject::call(List* args)
	{
		return std::invoke(this->native_func, args);
	}

	MemberFunctionKlass::MemberFunctionKlass()
	{
		this->set_klass_dict(new Dict());
	}

	MemberFunctionObject::MemberFunctionObject(FunctionObject* func) : owner(nullptr), func(func)
	{
		this->set_klass(MemberFunctionKlass::get_instance());
	}

	MemberFunctionObject::MemberFunctionObject(FunctionObject* func, Object* owner) : owner(owner), func(func)
	{
		this->set_klass(MemberFunctionKlass::get_instance());
	}

    CellObject::CellObject(List *ls, int i)
    {
		this->table = ls;
		this->index = i;
		this->set_klass(CellKlass::get_instance());
    }

    Object *CellObject::value()
    {
        return this->table->get(this->index);
    }

    CellKlass::CellKlass()
    {
		this->set_klass_dict(new Dict());
		this->set_name(new String("cell"));
    }

}
