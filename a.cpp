#include <iostream>
#include <unordered_map>
#include <ranges>

#include <lv_cpp/meta/template_info.hpp>

using Hash = std::unordered_map<int, int>;


int main(int argc, const char** argv)
{
    Hash h;
    auto rg = h | std::views::keys; 

    using T = std::invoke_result_t<decltype(std::views::elements<0>), Hash&>;

    PrintTypeCategory(rg);
    PrintTypeInfo(T);

    std::cout << "HelloWorld\n";
}