#pragma once

#include "Singleton.hpp"

namespace python
{

    class StringTable : Singleton<StringTable>
    {
        static constexpr const char* names[] = {
            "__next__",
            "__module__",
            "__init__",
            "__add__",
            "__len__",
            "__call__",
            "__name__",
            "__iter__",
            "__repr__",
            "__getitem__",
            "__setitem__",
            "__getattr__",
            "__setattr__",
        };
    };
      

} 

