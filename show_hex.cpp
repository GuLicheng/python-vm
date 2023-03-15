#include "src/PrettyPrint.hpp"
#include <iterator>
#include <fstream>
#include <string>

int main(int argc, const char** argv)
{
    if (argc != 2)
    {
        std::cout << "Argument Error.\n";
        return 1;
    }

    const char* path = argv[1];

    std::ifstream ifs(path, std::ios_base::in | std::ios_base::binary);

    if (!ifs.is_open())
    {
        std::cout << "File not existed.\n";
        return 1;
    }

    std::string bytes = std::string(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );

    print_hex_code<>(bytes.data(), bytes.size());
    return 0;
}


