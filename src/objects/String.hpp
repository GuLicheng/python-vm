#pragma once

#include "Object.hpp"

#include <string_view>
#include <string>

namespace python
{
    class StringKlass : public Klass, public Singleton<StringKlass>
    {
    public:

        StringKlass() = default;

        void initialize();

        virtual Object* allocate_instance(Object* callable, List* args) override;

        virtual size_t size() override;

        virtual void print(Object* self) override;
        virtual Object* py__hash__(Object* self) override;
        virtual Object* py__deepcopy__(Object* self) override;
        virtual Object* py__len__(Object* self) override;

        virtual Object* py__add__(Object* self, Object* other) override;

        virtual Object* py__lt__(Object* self, Object* other) override;
        virtual Object* py__ne__(Object* self, Object* other) override;
        virtual Object* py__eq__(Object* self, Object* other) override;
        virtual Object* py__gt__(Object* self, Object* other) override;
        virtual Object* py__le__(Object* self, Object* other) override;
        virtual Object* py__ge__(Object* self, Object* other) override;

    };

    class String : public Object
    {
        std::string m_value;
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
            std::cout << m_value;
        }

        std::string_view sv() const;

    };

}

namespace python::native
{
    Object* string_upper(List* args);
}

/*
['__repr__',
 '__hash__',
 '__str__',
 '__getattribute__',
 '__lt__',
 '__le__',
 '__eq__',
 '__ne__',
 '__gt__',
 '__ge__',
 '__iter__',
 '__mod__',
 '__rmod__',
 '__len__',
 '__getitem__',
 '__add__',
 '__mul__',
 '__rmul__',
 '__contains__',
 '__new__',
 '__format__',
 '__sizeof__',
 '__getnewargs__',
 '__doc__']
*/
