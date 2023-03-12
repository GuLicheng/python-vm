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

        virtual void print(Object* x) override;
        virtual Object* py__hash__(Object* x) override;
        virtual Object* py__deepcopy__(Object* x) override;
        virtual Object* py__len__(Object* x) override;

        virtual Object* py__add__(Object* x, Object* y) override;

        virtual Object* py__lt__(Object* x, Object* y) override;
        virtual Object* py__ne__(Object* x, Object* y) override;
        virtual Object* py__eq__(Object* x, Object* y) override;
        virtual Object* py__gt__(Object* x, Object* y) override;
        virtual Object* py__le__(Object* x, Object* y) override;
        virtual Object* py__ge__(Object* x, Object* y) override;

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

namespace python::native
{
    Object* string_upper(List* args);
}