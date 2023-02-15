#pragma once

#include "Integer.hpp"

namespace python
{

	struct Universe
	{

		inline static Integer* True = nullptr;

		inline static Integer* False = nullptr;

		inline static Object* None = nullptr;

		static void genesis();

		static void destroy();

	};


}
