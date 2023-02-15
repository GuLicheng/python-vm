#include "NativeFunction.hpp"
#include "Object.hpp"
#include "List.hpp"
#include "String.hpp"
#include "Universe.hpp"

#include <cctype>
#include <algorithm>
#include <ranges>

namespace python::native
{
	Object* len(List* args)
	{
		PYTHON_ASSERT(args->size() == 1);
		return args->get(0)->length();
	}

	Object* string_upper(List* args)
	{
		PYTHON_ASSERT(args && args->size() == 1);

		auto arg0 = args->get(0);
		PYTHON_ASSERT(arg0->is<String>());

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
		PYTHON_ASSERT(args && args->size() == 1);
		auto arg0 = args->get(0);

		PYTHON_ASSERT(arg0->is<String>());
		
		arg0->as<List>()->append(args->get(1));
		return Universe::None;

	}
}
