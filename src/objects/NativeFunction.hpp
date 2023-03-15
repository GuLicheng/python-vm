#pragma once

#include <initializer_list>

namespace python
{
    class List;
    class Object;
    class Dict;
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

namespace python
{
    enum struct MagicID
    {
        // convert
        PY__INT__,

        // binary_op
        PY__ADD__,
        PY__SUB__,
        PY__MUL__,
        PY__DIV__,

        // other
        PY__LEN__,
        PY__HASH__,
        PY__ITER__,
        PY__NEXT__,
    };

    void add_magic(Dict* dict, std::initializer_list<MagicID> magic_ids);

} 
