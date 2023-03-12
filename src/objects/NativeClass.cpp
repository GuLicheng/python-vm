#include "NativeClass.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"
#include "String.hpp"
#include "List.hpp"

namespace python
{
    /*
        class BaseException(object):
            def __init__(self, *args):
                self.args = args

        class Exception(BaseException): pass
        class StopInteration(Exception): pass
    */
    Dict* get_exception_constructor()
    {
        auto d = new Dict();
        auto ctor = FunctionObject::make_builtin_function(
            "__init__",
            FunctionObject::CO_VARARGS,
            nullptr,
            [](List* args) -> Object* {
                PYTHON_ASSERT(args && args->size() > 0 && "at least one argument(self)");
                auto arg0 = args->get(0);
                auto value = new List();
                for (int i = 1; i < args->size(); ++i)
                {
                    value->append(args->get(i));
                }
                arg0->py__setattr__(new String("args"), value);
                return arg0;
            }
        );

        d->put(new String("__init__"), ctor);
        return d;
    } 

}