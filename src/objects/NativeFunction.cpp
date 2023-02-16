#include "NativeFunction.hpp"
#include "Object.hpp"
#include "List.hpp"
#include "String.hpp"
#include "Universe.hpp"

#include <cctype>
#include <algorithm>
#include <ranges>

namespace python::native::detail
{
	template <typename T>
	Object* check_and_get_from_argument_list(List* args, int index, int argc)
	{
		PYTHON_ASSERT(args && args->size() == argc && "args should be available");
		auto arg = args->get(index);
		PYTHON_ASSERT(arg->is<T>());
		return arg;
	}
}

namespace python::native
{
	Object* len(List* args)
	{
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->length();
	}

	Object* string_upper(List* args)
	{
		auto arg0 = detail::check_and_get_from_argument_list<String>(args, 0, 1);

		// auto sv = arg0->as<String>()->value()
		// 	| std::views::transform(std::toupper)
		// 	| std::ranges::to<std::string>();

		auto sv = arg0->as<String>()->value();
		std::string result;
		for (char c : sv)
		{
			result += std::toupper(c);
		}

		return new String(std::move(result));

	}

	Object* list_append(List* args) 
	{
		auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 1);
		arg0->as<List>()->append(args->get(1));
		return Universe::None;
	}
    
	Object* list_pop(List* args)
	{
		auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 1);
		return arg0->as<List>()->pop();
	}

	Object* list_remove(List* args)
	{
		auto arg0 = detail::check_and_get_from_argument_list<List>(args, 0, 1);
		arg0->as<List>()->delete_by_object(args->get(1));
		return Universe::None;
	}

	// Object* list_reverse(List* args);
	// Object* list_sort(List* args);
	// Object* list_extend(List* args);
	// Object* list_index(List* args);
	// Object* list_getitem(List* args);


}
