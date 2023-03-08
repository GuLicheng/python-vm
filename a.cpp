#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <array>

template <int X, typename T> 
struct S
{
    S(T) { }
};

template <int X, typename T>
auto Make(T t)
{
    return S<X, T>(t);
}

int main(int argc, const char** argv)
{
    auto s = Make<1>(2);    
}