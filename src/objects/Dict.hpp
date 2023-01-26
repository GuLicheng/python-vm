#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"
#include "Integer.hpp"
#include "Universe.hpp"

#include <unordered_map>

namespace python
{
	// Helper class 
	namespace detail
	{
		struct ObjectKeyEqual
		{
			using is_transparent = void;

			/* static */ size_t operator()(Object* x) const
			{
				PYTHON_ASSERT(x && "x should not be nullptr");
				auto py_int = x->hash_code();
				// auto cpp_int = ((Integer*)py_int)->value();
				auto cpp_int = py_int->as<Integer>()->value();
				return static_cast<size_t>(cpp_int);
			}

			bool operator()(Object* x, Object* y) const
			{
				auto py_boolean = x->equal(y);
				return py_boolean == Universe::HiTrue ? true : false;
			}
		};
	}

	using PythonObjectDictionary = std::unordered_map<
		Object*,
		Object*,
		detail::ObjectKeyEqual,
		detail::ObjectKeyEqual
	>;


	class DictKlass : public Klass, public Singleton<DictKlass>
	{
	public:

		DictKlass();
		
	};

	class Dict : public Object
	{
		friend class DictKlass;

		PythonObjectDictionary* dict;

	public:

		using KlassType = DictKlass;

		Dict();

		Dict(PythonObjectDictionary* d);

		void put(Object* key, Object* value);

		Object* get(Object* key);

		bool has_key(Object* key);

		int size() const;

		Object* remove(Object* k);

	};

}

