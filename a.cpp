#include <iostream>
#include "src/internal/hashtable.hpp"
#include "src/internal/test.hpp"
#include <algorithm>
#include <string>

#include <vector>

int main()
{
    using Int = Int32<true, 2, 1000>;

    {
        python::collections::hash_set<Int, Int::HashType, std::equal_to<>, std::allocator<Int>> h;
        try
        {
            for (int i = 0; i < 50; ++i)
                h.emplace(i);
        }
        catch(...)
        {
        }
    }

    std::vector<int> vec;

    std::cout << Int::total_construct() << '\n';
    std::cout << Int::total_destruct() << '\n';

    std::cout << "Over\n";

}











