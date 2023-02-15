#include "Klass.hpp"
#include "Universe.hpp"
#include "TypeObject.hpp"
#include "String.hpp"
#include "List.hpp"
#include "Integer.hpp"
#include "Dict.hpp"
#include "StringTable.hpp"

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
        PYTHON_ASSERT(false && "");
    }

    void Klass::print(Object* obj)
    {
        Object* meth_repr = this->get_klass_attr(obj, StringTable::get_instance()->repr_str);
        if (meth_repr != Universe::None)
        {
            Interpreter::get_instance()->call_virtual(meth_repr, nullptr)->print();
            return;            
        }

        std::cout << "<object of";
        obj->get_klass()->get_name()->print();
        std::cout << ", at " << obj << '>';

    }

    Object* Klass::create_klass(Object* x, Object* supers, Object* name) {
        PYTHON_ASSERT(x->is<Dict>());
        PYTHON_ASSERT(supers->is<List>());
        PYTHON_ASSERT(name->is<String>());

        Klass* new_klass   = new Klass();
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