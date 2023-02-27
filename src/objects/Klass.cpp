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

        if (x->get_name()->__lt__(y->get_name()) == (Object*)Universe::True)
            return -1;
        else
            return 1;
    }

    void Klass::add_super(Klass* x)
    {
        if (!this->super)
            this->super = new List();
        this->super->append(x->type_object);
    }

    Object* Klass::get_super()
    {
        if (!this->super || this->super->size() == 0)
            return nullptr;

        // Return the first base class
        return this->super->get(0)->as<TypeObject>();
    }

    void Klass::order_supers()
    {
        if (this->super == NULL)
            return;

        if (this->mro == NULL)
            this->mro = new List();

        int cur = -1;
        for (int i = 0; i < this->super->size(); i++) {
            TypeObject* tp_obj = (TypeObject*)(this->super->get(i));
            Klass* k = tp_obj->get_own_klass();
            this->mro->append(tp_obj);
            if (k->mro == NULL)
                continue;

            for (int j = 0; j < k->mro->size(); j++) {
                TypeObject* tp_obj = (TypeObject*)(k->mro->get(j));
                int index = this->mro->index(tp_obj);
                if (index < cur) {
                    printf("Error: method resolution order conflicts.\n");
                    assert(false);
                }
                cur = index;

                if (index >= 0) {
                    this->mro->delete_by_index(index);
                }
                this->mro->append(tp_obj);
            }
        }

        if (this->mro == NULL)
            return;

        // printf("%s's mro is ", this->name->value());
        std::cout << this->name->value() << "'s mro is ";
        for (int i = 0; i < this->mro->size(); i++) {
            TypeObject* tp_obj = (TypeObject*)(this->mro->get(i));
            Klass* k = tp_obj->get_own_klass();
            // printf("%s, ", k->name->value());
            std::cout << k->name->value() << ", ";
        }
        std::cout << '\n';

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

        std::cout << "<object of";
        obj->get_klass()->get_name()->print();
        std::cout << ", at " << obj << '>';

    }

    Object* Klass::__lt__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::lt, x, y);
    }

    Object* Klass::__gt__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::gt, x, y);
    }

    Object* Klass::__le__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::le, x, y);
    }

    Object* Klass::__ge__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::ge, x, y);
    }

    Object* Klass::__add__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::add, x, y);
    }

    Object* Klass::__sub__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::sub, x, y);
    }

    Object* Klass::__mul__(Object* x, Object* y)
    {
        return find_magic_method_and_call(StringTable::mul, x, y);
    }

    Object* Klass::__hash__(Object* x)
    {
        return find_magic_method_and_call(StringTable::hash, x);
    }

    Object* Klass::__len__(Object* x)
    {
        return find_magic_method_and_call(StringTable::len, x);
    }

    Object* Klass::get_klass_attr(Object* x, Object* y)
    {
        auto result = find_in_parent(x, y);
        if (MemberFunctionObject::is_function(result))
            result = new MemberFunctionObject(result->as<FunctionObject>(), x);
        return result;
    }

    Object* Klass::__getattr__(Object* object, Object* attribute_name)
    {
        auto func = find_in_parent(object, StringTable::getattr);

        // Invoke __getattr__(self, attribute_name)
        if (func->klass == FunctionKlass::get_instance())
        {
            func = new MemberFunctionObject(func->as<FunctionObject>(), object);
            auto args = new List();
            args->append(attribute_name);
            return Interpreter::get_instance()->call_virtual(func, args);
        }

        auto result = Universe::None;
        if (object->obj_dict)
        {
            result = object->obj_dict->get(attribute_name);
            if (result != Universe::None)
                return result;
        }

        return this->get_klass_attr(object, attribute_name);

    }

    Object* Klass::__setattr__(Object* object, Object* key, Object* value)
    {
        auto func = object->klass->klass_dict->get(StringTable::setattr);

        // For member function, we bind the object and function
        if (func->klass == FunctionKlass::get_instance())
        {
            func = new MemberFunctionObject(func->as<FunctionObject>(), object);
            auto args = new List();
            args->append(key);
            args->append(value);
            // call __setattr__(self, key, value) 
            return Interpreter::get_instance()->call_virtual(func, args);
        }

        if (!object->obj_dict)
            object->init_dict();

        // Register attribute 
        object->obj_dict->put(key, value);
        return Universe::None;
    }

    std::size_t Klass::size()
    {
        return sizeof(Object);
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

    Object* Klass::find_and_call(Object* x, List* args, Object* function_name)
    {
        // Try to search function, if exited, invoke function with args, otherwise
        // throw.
        auto func = x->get_klass_attr(function_name);
        if (func != Universe::None)
            return Interpreter::get_instance()->call_virtual(func, args);
        std::cout << "class ";
        x->klass->name->print();
        std::cout << " Error: unsupported operation for class. ";
        PYTHON_ASSERT(false);
        return Universe::None;
    }

    Object* Klass::find_in_parent(Object* x, Object* y)
    {
        auto result = x->klass->klass_dict->get(y);

        if (result != Universe::None)
            return result;

        // Find attribute in all parents
        for (int i = 0; i < x->klass->mro->size(); ++i)
        {
            // Father class methods: x->klass->mro->get(i)->as<TypeObject>()->get_own_klass()
            result = x->klass->mro->get(i)->as<TypeObject>()->get_own_klass()->klass_dict->get(y);
            if (result != Universe::None)
                break;
        }

        return result;
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
        new_klass->order_supers();

        TypeObject* type_obj = new TypeObject();
        type_obj->set_own_klass(new_klass);

        return type_obj;
    }

}