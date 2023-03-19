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

    using NativeFunctionPointer = Object*(*)(List* args);
    // std::function<Object*(List*)>

    class FunctionKlass : public Klass, public Singleton<FunctionKlass>
    {
    public:

        FunctionKlass();

        virtual void print(Object* self) override;
    };

    class NativeFunctionKlass : public Klass, public Singleton<NativeFunctionKlass>
    {
    public:

        NativeFunctionKlass();

        virtual void print(Object* self) override;
    };

    class CellKlass : public Klass, public Singleton<CellKlass>
    {
    public:

        CellKlass();

        virtual void print(Object* self) override;
    };

    class MemberFunctionKlass : public Klass, public Singleton<MemberFunctionKlass>
    {
    public:

        MemberFunctionKlass();

        virtual void print(Object* self) override;
    };

    class FunctionObject : public Object
    {
        friend class FunctionKlass;
        friend class FrameObject;
        friend class Interpreter;
        friend class MemberFunctionObject;

        CodeObject* m_func_code;

        String* m_func_name;

        Dict* m_globals;  // global variable

        unsigned int m_flags;

        List* m_defaults; // default params

        List* m_closure;

        NativeFunctionPointer m_native_func;

    public:

        /*
            For function definition such as:
                def function(*args, **kwargs):
                    ...
                    yield ...
            Variadic arguments => args
            Variadic keyword => kwargs
            Generator => yield 
        */
        constexpr static int CO_VARARGS = 0x4;      
        constexpr static int CO_VARKEYWORDS = 0x8;  
        constexpr static int CO_GENERATOR = 0x20;   

        FunctionObject(Object* code_object);

        FunctionObject(Klass* code_object);
    
        FunctionObject(NativeFunctionPointer nfp);

        Object* call(List* args);

        virtual void print();

        static FunctionObject* make_builtin_function(std::string_view fname, unsigned int fflag, List* default_params, NativeFunctionPointer ffunc); 
    };

    class MemberFunctionObject : public Object
    {
        friend class MemberFunctionKlass;
        friend class Interpreter;

        Object* m_owner;

        FunctionObject* m_func; // For member function, the first argument is object 

    public:

        using KlassType = MemberFunctionKlass;

        explicit MemberFunctionObject(FunctionObject* func);

        MemberFunctionObject(FunctionObject* func, Object* owner);

        static bool is_function(Object* x);

        static bool is_yield_function(Object* x);
    };

    class CellObject : public Object
    {
        friend class CellKlass;

        List* m_table;   // the closure that this cell belong to

        int m_index;         // cell location
        
    public:

        using KlassType = CellKlass;

        CellObject(List* ls, int i);

        Object* value();
    };
}
