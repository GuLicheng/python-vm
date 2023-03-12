
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
        Integer* i3 = i1->py__add__(i2)->as<Integer>();
    
        assert(i3->value() == 1);

        // float/float
        Double* d1 = new Double(0.5);
        Double* d2 = new Double(0.5);
        Double* d3 = d1->py__add__(d2)->as<Double>();

        assert(d3->value() == 1.0);

        // float/int    
        Double* d4 = d1->py__add__(i1)->as<Double>();

        // int/float  
        Double* d5 = i1->py__add__(d1)->as<Double>();

        assert(d4->value() == d5->value());

        // string/string
        String* s1 = new String("Hello");
        String* s2 = new String("World");
        String* s3 = s1->py__add__(s2)->as<String>();

        assert(s3->value() == "HelloWorld");

        // list/list
        List* ls1 = new List();
        List* ls2 = new List();

        ls2->append(i1);
        ls2->append(d1);
        ls2->append(s1);

        List* ls3 = ls1->py__add__(ls2)->as<List>();

        assert(ls3->size() == 3);
        assert(ls3->get(0) == i1);
        assert(ls3->get(1) == d1);
        assert(ls3->get(2) == s1);

    }

    void test_sub()
    {
        // int/int
        Integer* i1 = new Integer(0);
        Integer* i2 = new Integer(1);
        Integer* i3 = i1->py__sub__(i2)->as<Integer>();
    
        assert(i3->value() == -1);

        // float/float
        Double* d1 = new Double(0.5);
        Double* d2 = new Double(0.5);
        Double* d3 = d1->py__sub__(d2)->as<Double>();

        assert(d3->value() == 0.0);

        // float/int    
        Double* d4 = d1->py__sub__(i1)->as<Double>();

        // int/float  
        Double* d5 = i1->py__sub__(d1)->as<Double>();

        assert(d4->value() + d5->value() == 0.0);
    }

    void test_mul()
    {
        // int/int
        Integer* i1 = new Integer(0);
        Integer* i2 = new Integer(1);
        Integer* i3 = i1->py__mul__(i2)->as<Integer>();
    
        assert(i3->value() == 0);


        // float/float
        Double* d1 = new Double(0.5);
        Double* d2 = new Double(0.5);
        Double* d3 = d1->py__mul__(d2)->as<Double>();

        assert(d3->value() == 0.5 * 0.5);
    
        // float/int    
        Double* d4 = d1->py__mul__(i1)->as<Double>();

        // int/float  
        Double* d5 = i1->py__mul__(d1)->as<Double>();

        assert(d4->value() == d5->value());
    
    }

    void test_hash_code()
    {

        auto check = []<typename T>(Object* o, T value) {
            return (std::size_t)o->py__hash__()->as<Integer>()->value() == std::hash<T>()(value);
        };

        Object* obj = nullptr;

        obj = new Integer(1);
        assert(check(obj, 1ll));

        std::string s = "HelloWorld";
        obj = new String(s);
        assert(check(obj, s));

        obj = new Double(3.14);
        assert(check(obj, 3.14));

    }

}




int main()
{
    python::test::test_member_type();
    python::test::test_add();
    python::test::test_sub();
    python::test::test_mul();
    python::test::test_hash_code();
    std::cout << "SimpleTest OK\n";
}



