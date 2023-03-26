#pragma once

#include "Object.hpp"

namespace python
{
    class SuperKlass : public Klass, public Singleton<SuperKlass>
    {
    public:

        SuperKlass();

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual void mark_self_and_children(Object* self) override;
    };

    class SuperObject : public Object
    {
        friend class SuperKlass;

        TypeObject* m_type;

        Object* m_instance;

    public:

        SuperObject(TypeObject* type, Object* instance);
    };
} // namespace python

