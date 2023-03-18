#include "Klass.hpp"
#include "Universe.hpp"
#include "TypeObject.hpp"
#include "String.hpp"
#include "List.hpp"
#include "Integer.hpp"
#include "Dict.hpp"
#include "StringTable.hpp"
#include "FunctionObject.hpp"

#include "../runtime/Interpreter.hpp"
#include "../Python.hpp"

namespace python
{

    int Klass::compare_klass(Klass* x, Klass* y) {
        if (x == y)
            return 0;

        if (x == IntegerKlass::get_instance())
            return -1;
        else if (y == IntegerKlass::get_instance())
            return 1;

        if (x->get_name()->py__lt__(y->get_name()) == (Object*)Universe::True)
            return -1;
        else
            return 1;
    }

    void Klass::show_klass_info()
    {
        if (!this->m_name)
        {
            std::cout << "Unknown class\n";
            return;
        }
        std::cout << "class name: " << this->m_name->value() << '\n';
        std::cout << "{\n";

        std::cout << "\tsupers: ";
        // for (auto s : this->klass_super)
        for (auto s : this->m_klass_mro)
        {
            std::cout << '\t' << s->m_name->value() << ',';
        }
        std::cout << "\n}\n";

    }

    void Klass::add_super(Klass* x)
    {
        // This function only called for build-in type.
        // We make sure that all of buildin's type only have one base class.
        this->m_klass_super.emplace_back(x);
        collect_super_klass(this->m_klass_mro, x);
    }

    Object* Klass::get_super()
    {
        NOT_IMPLEMENT;
        // if (!this->super || this->super->size() == 0)
        //     return nullptr;

        // // Return the first base class
        // return this->super->get(0)->as<TypeObject>();
    }

    // This function only used by user-defined class.
    void Klass::set_super_list(List* x)
    {
        // this->super = x; 
        PYTHON_ASSERT(x && "x should not be nullptr");
        for (int i = 0; i < x->size(); ++i)
        {
            // auto klass = this->super->get(i)->as<TypeObject>()->get_own_klass(); 
            auto klass = x->get(0)->as<TypeObject>()->get_own_klass();
            this->m_klass_super.emplace_back(klass);
            collect_super_klass(this->m_klass_mro, klass);
        }
        // In python 2.x, the usd-define class will not inherit from object. 
        // So we add this to make sure each usd-define class is inherited from object.
        if (this->m_klass_mro.empty())
        {
            this->m_klass_mro.emplace_back(ObjectKlass::get_instance());
        }
    }

    bool Klass::contains_mro(Klass* k)
    {
        return std::ranges::find(this->m_klass_mro, k) != this->m_klass_mro.end();
    }

    Object* Klass::allocate_instance(Object* callable, List* args)
    {
        Object* inst = new Object();
        inst->set_klass(callable->as<TypeObject>()->get_own_klass());
        Object* ctor = inst->get_klass_attr(StringTable::init);
        if (ctor != Universe::None)
        {
            Interpreter::get_instance()->call_virtual(ctor, args);
        }
        return inst;
    }

    void Klass::print(Object* obj)
    {
        // Object* meth_repr = this->get_klass_attr(obj, StringTable::get_instance()->repr_str);
        // if (meth_repr != Universe::None)
        // {
        //     Interpreter::get_instance()->call_virtual(meth_repr, nullptr)->print();
        //     return;            
        // }

        std::cout << "<object of ";
        obj->get_klass()->get_name()->print();
        std::cout << ", at " << obj << '>';

    }

    Object* Klass::py__lt__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::lt, self, other);
    }

    Object* Klass::py__gt__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::gt, self, other);
    }

    Object* Klass::py__le__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::le, self, other);
    }

    Object* Klass::py__ge__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::ge, self, other);
    }

    Object* Klass::py__add__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::add, self, other);
    }

    Object* Klass::py__sub__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::sub, self, other);
    }

    Object* Klass::py__mul__(Object* self, Object* other)
    {
        return find_magic_method_and_call(StringTable::mul, self, other);
    }

    Object* Klass::py__hash__(Object* self)
    {
        return find_magic_method_and_call(StringTable::hash, self);
    }

    Object* Klass::py__len__(Object* self)
    {
        return find_magic_method_and_call(StringTable::len, self);
    }

    Object* Klass::py__iter__(Object* self)
    {
        return find_magic_method_and_call(StringTable::iter, self);
    }

    Object* Klass::py__next__(Object* self)
    {
        return find_magic_method_and_call(StringTable::next, self);
    }

    Object* Klass::get_klass_attr(Object* x, Object* y)
    {
        auto result = find_in_parent(x, y);
        if (MemberFunctionObject::is_function(result))
            result = new MemberFunctionObject(result->as<FunctionObject>(), x);
        return result;
    }

    Object* Klass::py__getattr__(Object* object, Object* attribute_name)
    {
        auto func = find_in_parent(object, StringTable::getattr);

        // Invoke py__getattr__(self, attribute_name)
        // if (func->m_klass == FunctionKlass::get_instance())
        if (func->expected<FunctionKlass>())
        {
            func = new MemberFunctionObject(func->as<FunctionObject>(), object);
            auto args = new List();
            args->append(attribute_name);
            return Interpreter::get_instance()->call_virtual(func, args);
        }

        auto result = Universe::None;
        if (object->m_obj_dict)
        {
            result = object->m_obj_dict->get(attribute_name);
            if (result != Universe::None)
                return result;
        }

        return this->get_klass_attr(object, attribute_name);

    }

    Object* Klass::py__setattr__(Object* self, Object* key, Object* value)
    {
        auto func = self->m_klass->m_klass_dict->get(StringTable::setattr);

        // For member function, we bind the self and function
        // if (func->klass == FunctionKlass::get_instance())
        if (func->expected<FunctionKlass>())
        {
            func = new MemberFunctionObject(func->as<FunctionObject>(), self);
            auto args = new List();
            args->append(key);
            args->append(value);
            // call py__setattr__(self, key, value) 
            return Interpreter::get_instance()->call_virtual(func, args);
        }

        if (!self->m_obj_dict)
            self->init_dict();

        // Register attribute 
        self->m_obj_dict->put(key, value);
        return Universe::None;
    }

    Object* Klass::py__getitem__(Object* self, Object* name)
    {
        return find_magic_method_and_call(StringTable::getitem, self, name);
    }

    void Klass::py__setitem__(Object* self, Object* key, Object* value)
    {
        find_magic_method_and_call(StringTable::setitem, self, key, value);
    }

    std::size_t Klass::size()
    {
        return sizeof(Object);
    }

    void Klass::build_klass(std::string_view class_name, Klass* super_class, Dict* class_attributes)
    {
        // I am not sure whether it is necessary to initialize class_attributes.
        // Is nullptr OK for some special class ?
        if (!class_attributes)
            class_attributes = new Dict();

        this->m_klass_dict = class_attributes;
        (new TypeObject)->set_own_klass(this);
        this->set_name(new String(class_name.data(), class_name.size()));
        PYTHON_ASSERT(super_class);
        this->add_super(super_class);
    }

    Object* Klass::find_magic_method_and_call(Object* magic_method_name, Object* self)
    {
        PYTHON_ASSERT(magic_method_name->is<String>());
        return find_and_call(self, nullptr, magic_method_name);
    }

    Object* Klass::find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1)
    {
        PYTHON_ASSERT(magic_method_name->is<String>());
        auto args = new List();
        args->append(arg1); 
        return find_and_call(self, args, magic_method_name);
    }

    Object* Klass::find_magic_method_and_call(Object* magic_method_name, Object* self, Object* arg1, Object* arg2)
    {
        PYTHON_ASSERT(magic_method_name->is<String>());
        auto args = new List();
        args->append(arg1); 
        args->append(arg2); 
        return find_and_call(self, args, magic_method_name);
    }

    Object* Klass::find_and_call(Object* x, List* args, Object* function_name)
    {
        // Try to search function, if exited, invoke function with args, otherwise
        // throw.
        auto func = x->get_klass_attr(function_name);
        if (func != Universe::None)
            return Interpreter::get_instance()->call_virtual(func, args);
        std::cout << "class ";
        x->m_klass->m_name->print();
        std::cout << " Error: unsupported operation for class. ";
        PYTHON_ASSERT(false);
        return Universe::None;
    }

    Object* Klass::find_in_parent(Object* x, Object* y)
    {
        auto result = x->m_klass->m_klass_dict->get(y);

        if (result != Universe::None)
            return result;

        // if (!x->klass->mro)
        //     return result;

        // Find attribute in all parents
        // for (int i = 0; i < x->klass->mro->size(); ++i)
        // {
        //     // Father class methods: x->klass->mro->get(i)->as<TypeObject>()->get_own_klass()
        //     result = x->klass->mro->get(i)->as<TypeObject>()->get_own_klass()->klass_dict->get(y);
        //     if (result != Universe::None)
        //         break;
        // }

        // Replace above code with follow:
        for (size_t i = 0; i < x->m_klass->m_klass_super.size(); ++i)
        {
            // Father class methods: x->klass->mro->get(i)->as<TypeObject>()->get_own_klass()
            auto own_klass = x->m_klass->m_klass_super[i];
            result = own_klass->m_klass_dict->get(y);
            if (result != Universe::None)
                break;
        }


        return result;
    }

    Klass::Klass()
    {
        Universe::register_klass(this);
        // std::cout << "Klass Created\n";
    }

    // FIXME:
    // This function only called by `add_super` and `set_super_list`
    void Klass::collect_super_klass(std::vector<Klass*>& result, Klass* klass)
    {
        if (std::ranges::find(result, klass) == result.end())
            result.emplace_back(klass);

        for (auto k : klass->m_klass_mro)
        {
            auto location = std::ranges::find(result, k);
            if (location == result.end())
            {
                result.emplace_back(k);
                collect_super_klass(result, k);
            }    
            else
            {
                std::ranges::rotate(location, location + 1, result.end());
            }
        }
    }

    /*
        class A(object):
            number = 1

            def __init__(self, v):
                self.value = v

        x: { 
            "__init__": <Function: __init__>
            "number": 1, 
            "__module__": "__main__" 
        }
        supers: [object]
        name: A
    */
    Object* Klass::create_klass(Object* x, Object* supers, Object* name)
    {
        PYTHON_ASSERT(x->is<Dict>());
        PYTHON_ASSERT(supers->is<List>());
        PYTHON_ASSERT(name->is<String>());

        Klass* new_klass = new Klass();
        Dict* klass_dict = x->as<Dict>();
        List* supers_list = supers->as<List>();
        new_klass->set_klass_dict(klass_dict);
        new_klass->set_name(name->as<String>());
        new_klass->set_super_list(supers_list);

        TypeObject* type_obj = new TypeObject();
        type_obj->set_own_klass(new_klass);

        return type_obj;
    }

}