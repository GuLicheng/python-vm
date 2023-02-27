#pragma once

#include "Object.hpp"

#include <string_view>
#include <string>

namespace python
{

	class StringKlass : public Klass, public Singleton<StringKlass>
	{
	public:

		StringKlass();

		void initialize();

    	virtual Object* allocate_instance(Object* callable, List* args) override;

		virtual size_t size() override;

		virtual void print(Object* x) override;
		virtual Object* __hash__(Object* x) override;
		virtual Object* __deepcopy__(Object* x) override;
		virtual Object* __len__(Object* x) override;

		virtual Object* __add__(Object* x, Object* y) override;

		virtual Object* __lt__(Object* x, Object* y) override;
		virtual Object* __ne__(Object* x, Object* y) override;
		virtual Object* __eq__(Object* x, Object* y) override;
		virtual Object* __gt__(Object* x, Object* y) override;
		virtual Object* __le__(Object* x, Object* y) override;
		virtual Object* __ge__(Object* x, Object* y) override;

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
