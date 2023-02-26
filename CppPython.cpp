#include "src/reader/BufferedInputStream.hpp"
#include "src/reader/BinaryFileParser.hpp"
#include "src/runtime/Interpreter.hpp"
#include "src/objects/Universe.hpp"

#include <iostream>
#include <fstream>

using namespace python;

void python_main()
{
    const char* files[] = {
        R"(../../test\hello.pyc)",
        R"(../../test\test_if.pyc)",
        R"(../../test\test_loop.pyc)",
        R"(../../test\test_float.pyc)",
        R"(../../test\test_function.pyc)",
        R"(../../test\test.pyc)",
        R"(../../test\test_type.pyc)",
        R"(../../test\test_class.pyc)",
        R"(../../test\test_derive.pyc)",
        R"(../../test\test_operator_overload.pyc)",
    };

    const char* file = files[9];
    auto stream = BufferedInputStream(file);

    std::fstream log { "a.txt" };

    log << stream << '\n';

    auto parser = BinaryFileParser(&stream);
    auto codeobj = parser.parse();

    std::cout << "==================Start running code==================\n";
    auto interpreter = Interpreter::get_instance();
    interpreter->run(codeobj);

    std::cout << "==================End running code==================\n";
}

void exec()
{
    // Initialization
    Universe::genesis();

    python_main();

    // Destroy
    Universe::destroy();
}


int main()
{
    exec();
}

