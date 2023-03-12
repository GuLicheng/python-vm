#include "src/reader/BufferedInputStream.hpp"
#include "src/reader/BinaryFileParser.hpp"
#include "src/runtime/Interpreter.hpp"
#include "src/objects/Universe.hpp"

#include "src/objects/TypeObject.hpp"
#include "src/objects/Klass.hpp"

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
        R"(../../test\test_function.pyc)",  // 4
        R"(../../test\test.pyc)",
        R"(../../test\test_type.pyc)",
        R"(../../test\test_class.pyc)",
        R"(../../test\test_derive.pyc)",     // 8
        R"(../../test\test_operator_overload.pyc)",
        R"(../../test\test_exception.pyc)",
        R"(../../test\test_iterator.pyc)",
        R"(../../test\test_generator.pyc)", // 12
        R"(D:\code\CppPython\test\test.pyc)",
    };

    // const char* file = files[10];
    const char* file = files[12+1];
    auto stream = BufferedInputStream(file);

    std::fstream log { "a.txt" };

    log << stream << '\n';

    auto parser = BinaryFileParser(&stream);
    auto codeobj = parser.parse();

    std::cout << "==================Start running code==================\n";
    auto interpreter = Interpreter::get_instance();
    interpreter->run(codeobj);

    std::cout << "==================End running code==================\n";

    constexpr auto show_detail = [](python::Klass* klass) {
        std::cout << "Name = " << klass->get_name()->value();
        if (klass->get_mro())
        {
            std::cout << " Super class is : ";
            for (int i = 0; i < klass->get_mro()->size(); i++) 
            {
                auto tp_obj = (python::TypeObject*)(klass->get_mro()->get(i));
                auto k = tp_obj->get_own_klass();
                // printf("%s, ", k->name->value());
                std::cout << k->get_name()->value() << ", ";
            }
        }
        std::cout << '\n';
    };

    std::ranges::for_each(Universe::klasses, [=](python::Klass* klass) {
        klass->show_klass_info();
        // if (klass->get_name())
        //     show_detail(klass);
        // else
        //     std::cout << "Unknown class\n";
    });

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

