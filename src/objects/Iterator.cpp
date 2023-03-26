#include "Iterator.hpp"

namespace python
{
    void ObjectViewKlass::mark_self_and_children(Object* self)
    {
        if (self->is_marked())
            return;
        self->mark();
        self->as<ObjectView>()->m_iterable->mark_self_and_children();
    }
}

