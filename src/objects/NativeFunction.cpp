#include "List.hpp"
#include "Object.hpp"
#include "NativeFunction.hpp"
#include "String.hpp"
#include "Universe.hpp"
#include "TypeObject.hpp"
#include "Integer.hpp"
#include "Dict.hpp"
#include "Iterator.hpp"
#include "NativeClass.hpp"
#include "../runtime/Interpreter.hpp"
#include "../Python.hpp"

#include <cctype>
#include <algorithm>
#include <ranges>



namespace python::native
{
    Object* len(List* args)
    {
        PYTHON_ASSERT(args->size() == 1);
        return args->get(0)->py__len__();
    }

    Object* isinstance(List *args)
    {
        NOT_IMPLEMENT;
        // detail::check_argument_count(args, -2);

        // Object* instance = args->get(0);
        // Object* types = args->get(1);

        // auto& inner_list = args->value();


        // auto klass = instance->get_klass();

        // for (int i = 1; i < args->size(); ++i)
        // {
        //     auto type_klass = args->get(i)->get_klass();
        //     if (type_klass->contains_mro(klass))
        //         return Universe::True; 
        // }

        // return Universe::False;
    }

    Object* type(List* args)
    {
        auto arg0 = args->get(0);
        return arg0->get_klass()->get_type_object();
    }

    Object* hash(List* args)
    {
        PYTHON_ASSERT(args->size() == 1);
        return args->get(0)->py__hash__();
    }

    Object* iter(List* args)
    {
        PYTHON_ASSERT(args->size() == 1);
        return args->get(0)->py__iter__();
    }

    Object* next(List* args)
    {
        PYTHON_ASSERT(args->size() == 1);
        auto val =  args->get(0)->py__next__();
        if (!val)
        {
            auto stop_iteration = StopIterationKlass::get_instance()->get_type_object();
            Interpreter::get_instance()->do_raise(stop_iteration, nullptr, nullptr);
        }
        return val;
    }

    Object* map(List* args)
    {
        auto make_callable = [](Object* callable) {
            return [=](Object* object) {
                auto args = new List();
                args->append(object); 
                return Interpreter::get_instance()->call_virtual(callable, args);
            };
        };

        auto view = new ObjectView(args->get(1));
        auto transfer = make_callable(args->get(0));

        return new PyView(
            view, 
            (*view) | std::views::transform(transfer),
            "map_iterator"
        );
    }

    Object* filter(List* args)
    {
        auto make_callable = [](Object* callable) {
            return [=](Object* object) {
                auto args = new List();
                args->append(object); 
                auto result = Interpreter::get_instance()->call_virtual(callable, args);
                if (result == Universe::True)
                    return true;
                else if (result == Universe::False)
                    return false;
                else
                {
                    PYTHON_ASSERT(false && "pred should return boolean");
                    std::terminate();
                }
            };
        };

        auto view = new ObjectView(args->get(1));
        auto pred = make_callable(args->get(0));

        return new PyView(
            view, 
            (*view) | std::views::filter(pred),
            "filter_iterator"
        );
    }

    Object* take(List* args)
    {
        auto view = new ObjectView(args->get(1));
        auto count = args->get(0)->as<Integer>()->value();

        return new PyView(
            view,
            (*view) | std::views::take(count),
            "take_iterator"
        );
    }

    Object* drop(List* args)
    {
        auto view = new ObjectView(args->get(1));
        auto count = args->get(0)->as<Integer>()->value();

        return new PyView(
            view,
            (*view) | std::views::drop(count),
            "drop_iterator"
        );
    }

}
