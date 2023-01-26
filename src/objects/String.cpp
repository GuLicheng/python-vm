#include "String.hpp"
#include "Integer.hpp"
#include "Operation.hpp"

namespace python
{ 
	//String::String(const char* s) : str(s)
	//{
	//	this->set_klass(StringKlass::get_instance());
	//}

	String::String(const char* s, int length) : str(s, length)
	{
		this->set_klass(StringKlass::get_instance());
	}

	String::String(std::string s) : str(std::move(s))
	{
		this->set_klass(StringKlass::get_instance());
	}

	const char* String::c_str() const
	{
		return this->str.c_str();
	}

	const std::string& String::value() const
	{
		return this->str;
	}

	int String::length() const
	{
		return this->str.size();
	}

	std::string_view String::sv() const
	{
		return std::string_view(this->str);
	}

	void StringKlass::print(Object* x)
	{
		PYTHON_ASSERT(x && x->is<String>());
		auto s = (String*)x;
		std::cout << s->sv();
	}

	Object* StringKlass::hash_code(Object* x)
	{
		PYTHON_ASSERT(x && x->is<String>());
		auto sv = ((String*)x)->sv();
		auto hash_val = std::hash<std::string_view>()(sv);
		return detail::cpp_hash_value2py_int(hash_val);
	}

	Object* StringKlass::deepcopy(Object* x)
	{
		PYTHON_ASSERT(x && x->is<String>());
		auto str = (String*)x;
		return new String(str->c_str(), str->length());
	}

	Object* StringKlass::length(Object* x)
	{
		PYTHON_ASSERT(x && x->is<String>());
		String* s = (String*)x;
		return new Integer(static_cast<int64_t>(s->sv().size()));
	}

	Object* StringKlass::less(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::less<>(), x, y);
	}

	Object* StringKlass::not_equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::not_equal_to<>(), x, y);
	}

	Object* StringKlass::less_equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::less_equal<>(), x, y);
	}

	Object* StringKlass::equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::equal_to<>(), x, y);
	}

	Object* StringKlass::greater(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::greater<>(), x, y);
	}

	Object* StringKlass::greater_equal(Object* x, Object* y)
	{
		return detail::binary_relation_operation<String>(std::greater_equal<>(), x, y);
	}

	Object* StringKlass::add(Object* x, Object* y)
	{
		return detail::binary_arith_operation<String>(std::plus<>(), x, y);
	}

}