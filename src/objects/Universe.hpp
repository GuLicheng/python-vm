#pragma once

namespace python
{

    class Integer;
    class Object;
    class List;
    class Klass;

    struct Universe
    {

        inline static Integer* True = nullptr;

        inline static Integer* False = nullptr;

        inline static Object* None = nullptr;

        inline static List* klasses = nullptr;

        static void genesis();

        static void destroy();

    };


}
