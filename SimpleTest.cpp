
#include "src/objects/Object.hpp"
#include "src/objects/Double.hpp"
#include "src/objects/Integer.hpp"
#include "src/objects/Dict.hpp"
#include "src/objects/String.hpp"
#include "src/objects/List.hpp"
#include "src/objects/Operation.hpp"

#include <type_traits>
#include <concepts>
#include <string_view>
#include <assert.h>
#include <ranges>

namespace python::test
{
	void test_member_type()
	{
		static_assert(std::is_same_v<typename Object::KlassType, ObjectKlass>);
		static_assert(std::is_same_v<typename Double::KlassType, DoubleKlass>);
		static_assert(std::is_same_v<typename Integer::KlassType, IntegerKlass>);
		static_assert(std::is_same_v<typename Dict::KlassType, DictKlass>);
		static_assert(std::is_same_v<typename String::KlassType, StringKlass>);
		static_assert(std::is_same_v<typename List::KlassType, ListKlass>);
	}

	void test_add()
	{

		// int/int
		Integer* i1 = new Integer(0);
		Integer* i2 = new Integer(1);
		Integer* i3 = i1->add(i2)->as<Integer>();
	
		assert(i3->value() == 1);

		// float/float
		Double* d1 = new Double(0.5);
		Double* d2 = new Double(0.5);
		Double* d3 = d1->add(d2)->as<Double>();

		assert(d3->value() == 1.0);

		// float/int	
		Double* d4 = d1->add(i1)->as<Double>();

		// int/float  
		Double* d5 = i1->add(d1)->as<Double>();

		assert(d4->value() == d5->value());

		// string/string
		String* s1 = new String("Hello");
		String* s2 = new String("World");
		String* s3 = s1->add(s2)->as<String>();

		assert(s3->value() == "HelloWorld");

		// list/list

	}

	void test_sub()
	{
		// int/int
		Integer* i1 = new Integer(0);
		Integer* i2 = new Integer(1);
		Integer* i3 = i1->sub(i2)->as<Integer>();
	
		assert(i3->value() == -1);

		// float/float
		Double* d1 = new Double(0.5);
		Double* d2 = new Double(0.5);
		Double* d3 = d1->sub(d2)->as<Double>();

		assert(d3->value() == 0.0);

		// float/int	
		Double* d4 = d1->sub(i1)->as<Double>();

		// int/float  
		Double* d5 = i1->sub(d1)->as<Double>();

		assert(d4->value() + d5->value() == 0.0);
	}

	void test_mul()
	{
		// int/int
		Integer* i1 = new Integer(0);
		Integer* i2 = new Integer(1);
		Integer* i3 = i1->mul(i2)->as<Integer>();
	
		assert(i3->value() == 0);

		// float/float
		Double* d1 = new Double(0.5);
		Double* d2 = new Double(0.5);
		Double* d3 = d1->mul(d2)->as<Double>();

		assert(d3->value() == 0.5 * 0.5);
	
		// float/int	
		Double* d4 = d1->mul(i1)->as<Double>();

		// int/float  
		Double* d5 = i1->mul(d1)->as<Double>();

		assert(d4->value() == d5->value());
	
	}



}




int main()
{
	python::test::test_member_type();
	python::test::test_add();
	python::test::test_sub();
	python::test::test_mul();
    std::cout << "SimpleTest OK\n";
}



