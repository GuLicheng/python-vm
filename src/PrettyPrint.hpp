#pragma once

#include <iostream>
#include <string_view>
#include <iomanip>

template <typename T>
void print_label_style(const char* label_name, const T& val, int tab_cnt = 0, bool endline = true)
{
    for (int i = 0; i < tab_cnt; ++i)
        std::cout << '\t';
    
    std::cout << '<' << label_name << '>' << val << "</" << label_name << '>';

    if (endline)
        std::cout << '\n';
}

template <typename Fn> 
void print_label_style_fn(const char* label_name, Fn fn, int tab_cnt = 0, bool endline = true)
{
    for (int i = 0; i < tab_cnt; ++i)
        std::cout << '\t';
    
    std::cout << '<' << label_name << '>';
    fn(); 
    std::cout << "</" << label_name << '>';

    if (endline)
        std::cout << '\n';
}


template <int N = 16>
void print_hex_code(const char* bytes, int length)
{
    // std::string ascii;
    // ascii.resize(N);

    char ascii[N];

    int j;

    for (int i = 0; i < length; ++i)
    {
        std::cout.setf(std::ios::hex, std::ios::basefield);
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << (bytes[i] & 0xFF) << ' ';
        ascii[i % N] = bytes[i];
    
        if (i % N == (N - 1))
        {
            for (j = 0; j < N; ++j)
            {
                char c = ::isprint(ascii[j]) ? ascii[j] : '.';
                std::cout << std::dec << c;
            }

            std::cout << '\n';
        }
    }

    int rest = N - (length % N);

    // std::cout << "rest" << rest << '\n';

    if (rest)
    {
        for (int i = 0; i < rest; ++i)
        {
            std::cout << "   ";
        }
        for (int i = 0; i < length % N; ++i)
        {
            char c = ::isprint(ascii[i]) ? ascii[i] : '.';
            std::cout << std::dec << c;
        }
    }

}
