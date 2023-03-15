#include "String.hpp"
#include "Integer.hpp"
#include "Operation.hpp"
#include "TypeObject.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"

namespace python
{ 
    String::String(const char* s, int length) : m_value(s, length)
    {
        this->klass = StringKlass::get_instance();
    }

    String::String(std::string s) : m_value(std::move(s))
    {
        this->klass = StringKlass::get_instance();
    }

    const char* String::c_str() const
    {
        return this->m_value.c_str();
    }

    const std::string& String::value() const
    {
        return this->m_value;
    }

    int String::length() const
    {
        return this->m_value.size();
    }

    std::string_view String::sv() const
    {
        return std::string_view(this->m_value);
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

    void StringKlass::print(Object* self)
    {
        PYTHON_ASSERT(self && self->is<String>());
        auto s = (String*)self;
        std::cout << s->sv();
    }

    Object* StringKlass::py__hash__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<String>());
        auto sv = ((String*)self)->sv();
        auto hash_val = std::hash<std::string_view>()(sv);
        return detail::cpp_hash_value2py_int(hash_val);
    }

    Object* StringKlass::py__deepcopy__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<String>());
        auto str = (String*)self;
        return new String(str->c_str(), str->length());
    }

    Object* StringKlass::py__len__(Object* self)
    {
        PYTHON_ASSERT(self && self->is<String>());
        String* s = (String*)self;
        return new Integer(static_cast<int64_t>(s->sv().size()));
    }

    Object* StringKlass::py__lt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::less<>(), self, other);
    }

    Object* StringKlass::py__ne__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::not_equal_to<>(), self, other);
    }

    Object* StringKlass::py__le__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::less_equal<>(), self, other);
    }

    Object* StringKlass::py__eq__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::equal_to<>(), self, other);
    }

    Object* StringKlass::py__gt__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::greater<>(), self, other);
    }

    Object* StringKlass::py__ge__(Object* self, Object* other)
    {
        return detail::binary_relation_operation<String>(std::greater_equal<>(), self, other);
    }

    Object* StringKlass::py__add__(Object* self, Object* other)
    {
        return detail::binary_arith_operation<String>(std::plus<>(), self, other);
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
