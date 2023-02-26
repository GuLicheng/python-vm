#pragma once

#include <fstream>

namespace python
{
    class Logger
    {

        std::fstream fout;

    public:

        Logger(const char* filename) : fout(filename, std::ios_base::binary | std::ios_base::out)
        {
        }

        


    };

} // namespace python

