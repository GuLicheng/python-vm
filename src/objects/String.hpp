#pragma once

#include "Object.hpp"

#include <string_view>
#include <string>

namespace python
{

	class StringKlass : public Klass, public Singleton<StringKlass>
	{
	public:

		virtual void print(Object* x) override;
		virtual Object* hash_code(Object* x) override;
		virtual Object* deepcopy(Object* x) override;
		virtual Object* length(Object* x) override;

		virtual Object* add(Object* x, Object* y) override;

		virtual Object* less(Object* x, Object* y) override;
		virtual Object* not_equal(Object* x, Object* y) override;
		virtual Object* equal(Object* x, Object* y) override;
		virtual Object* greater(Object* x, Object* y) override;
		virtual Object* less_equal(Object* x, Object* y) override;
		virtual Object* greater_equal(Object* x, Object* y) override;

	};

	class String : public Object
	{
		std::string str;
	public:

		using KlassType = StringKlass;

		//String(const char* s);

		String(const char* s, int length);

		explicit String(std::string s);

		const char* c_str() const;

		const std::string& value() const;

		int length() const;

		void show() override
		{
			std::cout << str;
		}

		std::string_view sv() const;

	};

}
