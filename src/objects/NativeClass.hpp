#pragma once

#include "Object.hpp"

#define CreateExceptionClass(class_name, base_class_name) \
    class class_name##Klass : public Klass, public Singleton<class_name##Klass> \
    { \
    public: \
        class_name##Klass() \
        { \
            this->build_klass(#class_name , base_class_name##Klass :: get_instance(), get_exception_constructor());  \
        } \
        virtual void mark_self_and_children(Object* self) override { self->mark(); } \
    }

namespace python
{
    Dict* get_exception_constructor();

    CreateExceptionClass(BaseException, Object);

    CreateExceptionClass(Exception, BaseException);

    CreateExceptionClass(StopIteration, Exception);

    CreateExceptionClass(AssertionError, Exception);
} 

#undef CreateExceptionClass