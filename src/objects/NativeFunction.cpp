#include "List.hpp"
#include "Object.hpp"
#include "NativeFunction.hpp"
#include "String.hpp"
#include "Universe.hpp"
#include "TypeObject.hpp"
#include "Integer.hpp"
#include "Dict.hpp"
#include "../Python.hpp"

#include <cctype>
#include <algorithm>
#include <ranges>



namespace python::native
{
	Object* len(List* args)
	{
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->__len__();
	}

    Object* isinstance(List *args)
    {
		Object* instance = args->get(0);
		Object* types = args->get(1);

		PYTHON_ASSERT(types->is<TypeObject>());

		Klass* k = instance->get_klass();

		if (k->get_type_object() == types)
			return Universe::True;

		// for (int i = 0; i < k->mro()->size(); i++) {
		// 	if (k->mro()->get(i) == y) {
		// 		return Universe::HiTrue;
		// 	}
		// }

		return Universe::False;
    }

    Object* type(List* args)
    {
        auto arg0 = args->get(0);
		return arg0->get_klass()->get_type_object();
    }

    Object* hash(List* args)
    {
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->__hash__();
    }

    Object* iter(List* args)
    {
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->__iter__();
    }

    Object* next(List* args)
    {
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->__next__();
    }
}
