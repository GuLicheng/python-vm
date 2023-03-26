#pragma once

#include <memory>

namespace python
{
    template <typename T>
    class PyAllocator : public std::allocator<T>
    {
        
    };
} // namespace python

