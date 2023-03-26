#pragma once

#include "Common.hpp"

#include <vector>

namespace python
{
    template <typename TValue, typename TAllocator = Allocator<TValue>>
    using Vector = std::vector<TValue, TAllocator>;
} // namespace python

