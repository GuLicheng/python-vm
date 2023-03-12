#pragma once

#include <vector>

namespace python
{

    class Integer;
    class Object;
    class List;
    class Klass;

    struct Universe
    {

        // In python, True and False is type bool
        // and None is NoneType

        inline static Integer* True = nullptr;

        inline static Integer* False = nullptr;

        inline static Object* None = nullptr;

        // inline static List* klasses = nullptr;

        inline static std::vector<Klass*> klasses;

        static void register_klass(Klass* klass);

        static void genesis();

        static void destroy();

    };


}
