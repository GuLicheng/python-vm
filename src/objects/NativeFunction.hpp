#pragma once

namespace python
{
    class List;
    class Object;
} // namespace python


namespace python::native
{

    /* Global Function */

    Object* len(List* args);

    Object* isinstance(List* args);

    Object* type(List* args);

    Object* hash(List* args);

    Object* iter(List* args);

    Object* next(List* args);

    Object* map(List* args);

    Object* filter(List* args);

    Object* take(List* args);

    Object* drop(List* args);

}
