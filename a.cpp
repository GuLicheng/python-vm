#include <iostream>
#include <ranges>
#include <algorithm>
#include <string>
#include <cctype>

int main()
{
    std::string s = "As0W2Df";

    auto rg = s 
        | std::views::filter(::isalpha) 
        | std::views::reverse;
    
    std::ranges::for_each(rg, [](char& c) { c = ::tolower(c); });

    for (char c : s)
        std::cout << c << '\n';
}
