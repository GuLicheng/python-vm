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

        if (x->get_name()->less(y->get_name()) == (Object*)Universe::True)
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
        // Use DFS reorder super classes for quick find the overwrite function.
        if (!this->super)
            return;
        
        if (!this->mro)
            this->mro = new List();
        
        int cur = -1;
        for (int i = 0; i < this->super->size(); ++i)
        {
            auto tp_obj = this->super->get(i)->as<TypeObject>();
            auto k = tp_obj->get_own_klass();
            if (!k->mro)
                continue;
            
            for (int j = 0; j < k->mro->size(); ++j)
            {
                auto tp_obj = k->mro->get(j)->as<TypeObject>();
                int index = this->mro->index(tp_obj);
                if (index < cur)
                {
                    PYTHON_ASSERT(false && "Error: method resolution order conflicts.");
                }
                cur = index;
                if (index >= 0)
                    this->mro->delete_by_index(index);
                this->mro->append(tp_obj);
            }
        }   

        if (!this->mro)
            return;

        std::cout << this->name->value() << "'s mro is";
        for (int i = 0; i < this->mro->size(); ++i)
        {
            auto tp_obj = this->mro->get(i)->as<TypeObject>();
            auto k = tp_obj->get_own_klass();
            std::cout << k->get_name()->value() << ',';
        }
        std::cout << '\n';

    }

    Object* Klass::allocate_instance(Object* callable, List* args)
    {
        Object* inst = new Object();
        inst->set_klass(callable->as<TypeObject>()->get_own_klass());
        Object* ctor = inst->getattr(StringTable::get_instance()->init_str);
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

    Object* Klass::get_klass_attr(Object* x, Object* y)
    {
        auto result = this->find_in_parent(x, y);
        if (MemberFunctionObject::is_function(result))
            result = new MemberFunctionObject(result->as<FunctionObject>(), x);
        return result;
    }

    Object *Klass::getattr(Object *object, Object *attribute_name)
    {
        auto func = this->find_in_parent(object, StringTable::get_instance()->getattr_str);

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

    Object* Klass::setattr(Object* object, Object* key, Object* value)
    {
        auto func = object->klass->klass_dict->get(StringTable::get_instance()->setattr_str);

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