#pragma once

#include "Object.hpp"
#include "List.hpp"

namespace python
{
    /*
        For builtin type such as 'list', the attribute is read-only and
        we cannot register new attribute/member-field, so for non-user class, 
        the `m_obj_dict` is unnecessary.
    */
    class UserClass : public Object
    {
        Dict* m_obj_dict = nullptr;
    };
} // namespace python

