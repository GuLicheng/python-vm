#pragma once

#include "Integer.hpp"

namespace python
{

	struct Universe
	{

		inline static Integer* HiTrue = nullptr;

		inline static Integer* HiFalse = nullptr;

		inline static Object* HiNone = nullptr;

		static void genesis();

		static void destroy();

	};


}
