#pragma once

#include "Common.hpp"

#include <unordered_map>

namespace python
{
    template <typename TKey, typename TValue, 
        typename THash = std::hash<TKey>, 
        typename TKeyEq = std::equal_to<TKey>, 
        typename TAllocator = Allocator<std::pair<const TKey, TValue>>>
    using HashMap = std::unordered_map<TKey, TValue, THash, TKeyEq, TAllocator>;
} // namespace python

