#include "String.hpp"
#include "Integer.hpp"
#include "Operation.hpp"
#include "TypeObject.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"

namespace python
{ 

    String::String(const char* s, int length) : str(s, length)
    {
        this->klass = StringKlass::get_instance();
    }

    String::String(std::string s) : str(std::move(s))
    {
        this->klass = StringKlass::get_instance();
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

    void StringKlass::initialize()
    {
        Dict* klass_dict = new Dict();

        klass_dict->put(new String("upper"), new FunctionObject(native::string_upper));

        this->build_klass("str", ObjectKlass::get_instance(), klass_dict);
    }

    Object* StringKlass::allocate_instance(Object* callable, List* args)
    { 
        if (!args || args->size() == 0)
            return new String("");
        else
            return nullptr;
    }

    size_t StringKlass::size()
    {
        return sizeof(String);
    }

    void StringKlass::print(Object *x)
    {
        PYTHON_ASSERT(x && x->is<String>());
        auto s = (String*)x;
        std::cout << s->sv();
    }

    Object* StringKlass::py__hash__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<String>());
        auto sv = ((String*)x)->sv();
        auto hash_val = std::hash<std::string_view>()(sv);
        return detail::cpp_hash_value2py_int(hash_val);
    }

    Object* StringKlass::py__deepcopy__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<String>());
        auto str = (String*)x;
        return new String(str->c_str(), str->length());
    }

    Object* StringKlass::py__len__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<String>());
        String* s = (String*)x;
        return new Integer(static_cast<int64_t>(s->sv().size()));
    }

    Object* StringKlass::py__lt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::less<>(), x, y);
    }

    Object* StringKlass::py__ne__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::not_equal_to<>(), x, y);
    }

    Object* StringKlass::py__le__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::less_equal<>(), x, y);
    }

    Object* StringKlass::py__eq__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::equal_to<>(), x, y);
    }

    Object* StringKlass::py__gt__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::greater<>(), x, y);
    }

    Object* StringKlass::py__ge__(Object* x, Object* y)
    {
        return detail::binary_relation_operation<String>(std::greater_equal<>(), x, y);
    }

    Object* StringKlass::py__add__(Object* x, Object* y)
    {
        return detail::binary_arith_operation<String>(std::plus<>(), x, y);
    }

}

namespace python::native
{
    Object* string_upper(List* args)
    {
        auto arg0 = detail::check_and_get_from_argument_list<String>(args, 0, 1);

        // auto sv = arg0->as<String>()->value()
        //     | std::views::transform(std::toupper)
        //     | std::ranges::to<std::string>();

        auto sv = arg0->as<String>()->value();
        std::string result;
        for (char c : sv)
        {
            result += std::toupper(c);
        }
        return new String(std::move(result));
    }
}
