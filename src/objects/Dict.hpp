#pragma once

#include "Object.hpp"
#include "Klass.hpp"
#include "Singleton.hpp"
#include "Integer.hpp"
#include "Universe.hpp"
#include "Iterator.hpp"

#include <array>
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
				auto py_int = x->__hash__();
				auto cpp_int = py_int->as<Integer>()->value();
				return static_cast<size_t>(cpp_int);
			}

			bool operator()(Object* x, Object* y) const
			{
				auto py_boolean = x->__eq__(y);
				return py_boolean == Universe::True ? true : false;
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

		void initialize();

    	virtual Object* allocate_instance(Object* callable, List* args) override;

		virtual size_t size() override;

		virtual Object* __str__(Object* x) override;

		virtual Object* __contains__(Object* x, Object* y) override;

		virtual Object* __iter__(Object* x) override;

		virtual void print(Object* object) override;
		
	};

	class Dict : public Object
	{

		friend class DictKlass;

		PythonObjectDictionary dict;

	public:

		using KlassType = DictKlass;

		Dict();

		Dict(PythonObjectDictionary d);

		void put(Object* key, Object* value);

		Object* get(Object* key);

		bool has_key(Object* key);

		int size() const;

		Object* remove(Object* k);

		std::array<PythonObjectDictionary::iterator, 2> get_iterator_pair();

	};

	enum struct DictIteratorMode 
	{
		Keys, Values, Items
	};

	template <DictIteratorMode EMode> 
	struct DictIterator : PyIterator<Dict, PythonObjectDictionary::iterator>
	{

		using base = PyIterator<Dict, PythonObjectDictionary::iterator>;

		DictIterator(Dict* dict);

		Object* value();
	
	};

	template <DictIteratorMode EMode>
	class DictIteratorKlass 
		: public PyIteratorKlass<DictIterator<EMode>>,
		  public Singleton<DictIteratorKlass<EMode>>
	{
	public:

		DictIteratorKlass();

		void initialize();

		virtual void print(Object* x) override;

	};

}

