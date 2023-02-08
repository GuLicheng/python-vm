#include "src/internal/hashtable.hpp"

#include <ranges>
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <fstream>
#include <iostream>

inline constexpr auto default_num = 100'0000;
inline std::random_device rd;

std::vector<int> random_range_int(int n = default_num)
{
    auto random_generator = [&]() {
        return rd() % (default_num * 10);
    };
    std::vector<int> ret;
    ret.reserve(n);
    std::generate_n(std::back_inserter(ret), n, random_generator);
    return ret;
}

void test()
{
    python::collections::hash_set<int> s1;
    std::unordered_set<int> s2;

    auto v = random_range_int();


    for (int i = 0; i < v.size(); ++i)
    {
        auto mode = rd() % 3;

        switch (mode)
        {
        case 0: 
        {
            // insert
            auto [_1, succeed1] = s1.insert(v[i]);
            auto [_2, succeed2] = s2.insert(v[i]);
            assert(succeed1 == succeed2);
            break;
        }
        case 1:
        {
            assert(s1.contains(v[i]) == s2.contains(v[i]));
            break;
        }
        case 2:
        {
            auto i1 = s1.erase(v[i]);
            auto i2 = s2.erase(v[i]);
            assert(i1 == i2);
            break;
        }
        default: std::unreachable();
        }
    }

    assert(s1.size() == s2.size());

    std::set<int> order1 = { s1.begin(), s1.end() };
    std::set<int> order2 = { s2.begin(), s2.end() };
    assert(order1 == order2);
    std::cout << "Ok For Testing\n";
}


void test1()
{

    python::collections::hash_map<int, std::string> s1;

    {
        auto [iter1, succeed1] = s1.insert_or_assign(1, "Hello");
        auto [iter2, succeed2] = s1.insert_or_assign(1, "World");
        auto [iter3, succeed3] = s1.insert_or_assign(2, "World");

        assert(succeed1);
        assert(!succeed2);
        assert(succeed3);

        assert(s1.size() == 2);

        assert(s1.find(1)->second == "World");
        assert(s1.find(2)->second == "World");
    }

    {
        auto [iter1, succeed1] = s1.try_emplace(3, "Hello");
        auto [iter2, succeed2] = s1.try_emplace(3, "World");
        auto [iter3, succeed3] = s1.try_emplace(4, "World");

        assert(succeed1);
        assert(!succeed2);
        assert(succeed3);

        assert(s1.size() == 4);

        assert(s1.find(3)->second == "Hello");
        assert(s1.find(4)->second == "World");
    }

    for (const auto& [k, v] : s1)
    {
        std::cout << "Key = " << k << " Value = " << v << '\n';
    }

    python::collections::hash_map<std::string, int> s2;

    s2.insert_or_assign("Hello", 1);
    s2.insert_or_assign("World", 2);

    for (const auto& [k, v] : s2)
    {
        std::cout << "Key = " << k << " Value = " << v << '\n';
    }
}


int main()
{
    std::cout << "Start\n";
    test1();
    test();
    std::cout << "End\n";

}

