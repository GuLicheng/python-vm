#pragma once

#include "../Python.hpp"

#include <string_view>
#include <vector>

#define NOT_IMPLEMENT PYTHON_ASSERT(false && "not implement"); return 0 

namespace python
{
    class Object;
    class String;
    class Dict;
    class TypeObject;
    class List;

    class Klass
    {

        friend class TypeKlass;
        friend class MemberFunctionObject;

    protected:

        // Since each class only have one Klass, so
        // we use a vector to save it's super 
        std::vector<Klass*> klass_super;

        std::vector<Klass*> klass_mro;

        List* super = nullptr;  // super classes

        List* mro = nullptr;    // memory resolution order, control the process of methods searching

        TypeObject* type_object = nullptr;

        String* name = nullptr;
        
        Dict* klass_dict = nullptr;

        friend class Object;

        void build_klass(std::string_view class_name, Klass* super_class, Dict* class_attributes);

        static Object* find_magic_method_and_call(Object* magic_method_name, Object* self);

        static Object* find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1);

        static Object* find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1, Object* arg2);

        static Object* find_and_call(Object* x, List* args, Object* function_name);

        static Object* find_in_parent(Object* x, Object* y);
        
        static void collect_super_klass(std::vector<Klass*>& result, Klass* klass);

    public:

        Klass();

        Klass(const Klass&) = delete;

        static Object* create_klass(Object* x, Object* supers, Object* name);

        static int compare_klass(Klass* x, Klass* y);

        void show_klass_info();

        // Super
        void add_super(Klass* x);
        
        Object* get_super();
        
        void order_supers();

        void set_super_list(List* x);

        // Mro
        List* get_mro();

        bool contains_mro(Klass* k);

        // Default __init__
        virtual Object* allocate_instance(Object* callable, List* args);

        // Name
        void set_name(String* x) { this->name = x; }

        String* get_name() const { return this->name; }

        // Klass
        void set_klass_dict(Dict* d) { this->klass_dict = d; }

        Dict* get_klass_dict() { return this->klass_dict; }

        // Type
        void set_type_object(TypeObject* obj) { this->type_object = obj; }

        TypeObject* get_type_object() { return this->type_object; }

        // Member function
        virtual void print(Object* obj);


        // Magic methods

        // Operators
        // ==, !=, <, >, <=, >=
        virtual Object* py__eq__(Object* x, Object* y) { NOT_IMPLEMENT; }
        virtual Object* py__ne__(Object* x, Object* y) { NOT_IMPLEMENT; }
        virtual Object* py__lt__(Object* x, Object* y);
        virtual Object* py__gt__(Object* x, Object* y);
        virtual Object* py__le__(Object* x, Object* y);
        virtual Object* py__ge__(Object* x, Object* y);

        // +, -, *, /, //, %
        virtual Object* py__add__(Object* x, Object* y);
        virtual Object* py__sub__(Object* x, Object* y);
        virtual Object* py__mul__(Object* x, Object* y);
        virtual Object* py__div__(Object* x, Object* y) { NOT_IMPLEMENT; } //  operator /
        virtual Object* py__floordiv__(Object* x, Object* y) { NOT_IMPLEMENT; } //   operator //
        virtual Object* py__mod__(Object* x, Object* y) { NOT_IMPLEMENT; }
    
        virtual Object* py__contains__(Object* x, Object* y) { NOT_IMPLEMENT; }

        // Type convert str, int, float, bool, ...
        virtual Object* py__str__(Object* x) { NOT_IMPLEMENT; }
        virtual Object* py__int__(Object* x) { NOT_IMPLEMENT; }
        virtual Object* py__float__(Object* x) { NOT_IMPLEMENT; }
        virtual Object* py__bool__(Object* x) { NOT_IMPLEMENT; }


        virtual Object* py__hash__(Object* x);
        virtual Object* py__deepcopy__(Object* x) { NOT_IMPLEMENT; }
        virtual Object* py__len__(Object* x);
        virtual Object* py__iter__(Object* x);
        virtual Object* py__next__(Object* x);


        virtual Object* get_klass_attr(Object* x, Object* y);

        virtual Object* py__getattr__(Object* object, Object* attribute_name);
        virtual Object* py__setattr__(Object* object, Object* key, Object* value);
        virtual Object* py__getitem__(Object* object, Object* name);
        virtual void py__setitem__(Object* object, Object* key, Object* value);

        // gc interface
        // virtual void oops_do(class Closure*, Object*);

        // class itself only
        virtual std::size_t size();
        // virtual void oops_do(class Closure*);

        // void* operator new(std::size_t);

    };

}
