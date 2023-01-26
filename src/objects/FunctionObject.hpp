#pragma once

#include "Klass.hpp"
#include "Object.hpp"
#include "Singleton.hpp"
#include "Dict.hpp"
#include "List.hpp"

namespace python
{


	class CodeObject;
	class String;
	class Closure;

	class FunctionKlass : public Klass, public Singleton<FunctionKlass>
	{
	public:

		virtual void print(Object* obj) override;

	};

	class NativeFunctionKlass : public Klass, public Singleton<NativeFunctionKlass>
	{

	};

	using NativeFunctionPointer = Object*(*)(List* args);
	// std::function<Object*(List*)>

	class CellKlass : public Klass, public Singleton<CellKlass>
	{
	public:

		CellKlass();

		// virtual void oops_do(Closure* closure, Object* obj);

		// virtual int size();

	};

	class MemberFunctionKlass : public Klass, public Singleton<MemberFunctionKlass>
	{
	public:

		MemberFunctionKlass();
	};


	class FunctionObject : public Object
	{
		friend class FunctionKlass;
		friend class FrameObject;
		friend class Interpreter;

		CodeObject* func_code;

		String* func_name;

		Dict* globals;  // global variable

		unsigned int flags;

		List* defaults; // default params

		List* closure;

		NativeFunctionPointer native_func;

	public:

		constexpr static int CO_VARARGS = 0x4;
		constexpr static int CO_VARKEYWORDS = 0x8;
		constexpr static int CO_GENERATOR = 0x20;

		FunctionObject(Object* code_object);

		FunctionObject(Klass* code_object);
	
		FunctionObject(NativeFunctionPointer nfp);

		//String* function_name() const { return this->func_name; }

		//int flags() const { return this->flags; }

		Object* call(List* args);

	};


	class MemberFunctionObject : public Object
	{
		friend class MemberFunctionKlass;
		friend class Interpreter;

		Object* owner;

		FunctionObject* func; // For member function, the first argument is object 

	public:

		using KlassType = MemberFunctionKlass;

		explicit MemberFunctionObject(FunctionObject* func);

		MemberFunctionObject(FunctionObject* func, Object* owner);

	};

	class CellObject : public Object
	{
		friend class CellKlass;

		List* table;   // the closure that this cell belong to

		int index;		 // cell location
		
	public:

		using KlassType = CellKlass;

		CellObject(List* ls, int i);

		Object* value();
	
	};

}
