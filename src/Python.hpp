#pragma once

#include <cstdlib>
#include <cassert>

namespace python
{
    inline void abort_process(bool x)
    {
        if (!x)
            std::abort();
    }
}

//#define PYTHON_ASSERT(x) (python::abort_process(x))
#define PYTHON_ASSERT(x) (assert(x))
