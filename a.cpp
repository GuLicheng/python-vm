#include <stdio.h>
#include <iostream>
#include <bitset>

enum Tag : int
{
    PY_ADD,
    PY_ITER
};

int main(int argc, const char** argv)
{
    std::bitset<35> flags;

    std::bitset<35> flag1;
    flag1.set(PY_ADD, true);
    flag1.set(PY_ITER, true);

    flags |= flag1;

    std::cout << flags << '\n';
}