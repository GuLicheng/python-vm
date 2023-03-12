#include "Object.hpp"
#include "Universe.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"
#include "String.hpp"
#include "Universe.hpp"

namespace python
{
    void Object::init_dict()
    {
        this->obj_dict = new Dict();
    }

    void Object::print()
    {
        // std::cout << "name = " << this->klass->get_name()->value() << '\n';
        this->klass->print(this);
    }

    Object* Object::py__add__(Object* x)
    {
        return this->klass->py__add__(this, x);
    }

    Object* Object::py__sub__(Object* x)
    {
        return this->klass->py__sub__(this, x);
    }

    Object* Object::py__mul__(Object* x)
    {
        return this->klass->py__mul__(this, x);
    }

    Object* Object::py__div__(Object* x)
    {
        return this->klass->py__div__(this, x);
    }

    Object* Object::py__floordiv__(Object* x)
    {
        return this->klass->py__floordiv__(this, x);
    }

    Object* Object::py__mod__(Object* x)
    {
        return this->klass->py__mod__(this, x);
    }

    Object* Object::py__gt__(Object* x)
    {
        return this->klass->py__gt__(this, x);
    }

    Object* Object::py__lt__(Object* x)
    {
        return this->klass->py__lt__(this, x);
    }

    Object* Object::py__eq__(Object* x)
    {
        return this->klass->py__eq__(this, x);
    }

    Object* Object::py__ne__(Object* x)
    {
        return this->klass->py__ne__(this, x);
    }

    Object* Object::py__le__(Object* x)
    {
        return this->klass->py__le__(this, x);
    }

    Object* Object::py__ge__(Object* x)
    {
        return this->klass->py__ge__(this, x);
    }

    Object* Object::py__contains__(Object* x)
    {
        return this->klass->py__contains__(this, x);
    }

    Object* Object::py__hash__()
    {
        return this->klass->py__hash__(this);
    }

    Object* Object::py__deepcopy__()
    {
        return this->klass->py__deepcopy__(this);
    }

    Object* Object::py__len__()
    {
        return this->klass->py__len__(this);
    }

    Object* Object::py__iter__()
    {
        return this->klass->py__iter__(this);
    }

    Object* Object::py__next__()
    {
        return this->klass->py__next__(this);
    }

    Object* Object::py__str__()
    {
        return this->klass->py__str__(this);
    }

    Object* Object::py__int__()
    {
        return this->klass->py__int__(this);
    }

    Object* Object::py__float__()
    {
        return this->klass->py__float__(this);
    }

    Object* Object::py__bool__()
    {
        return this->klass->py__bool__(this);
    }

    Object* Object::py__getattr__(Object* attribute)
    {
        return this->klass->py__getattr__(this, attribute);
    }

    void Object::py__setitem__(Object* key, Object* value)
    {
        this->klass->py__setitem__(this, key, value);
    }

    Object* Object::py__getitem__(Object* name)
    {
        return this->klass->py__getitem__(this, name);
    }

    Object* Object::get_klass_attr(Object* attribute)
    {
        return this->klass->get_klass_attr(this, attribute);
    }

    Object* Object::py__setattr__(Object* key, Object* value)
    {
        return this->klass->py__setattr__(this, key, value);
    }
    
    void ObjectKlass::print(Object* object)
    {
        if (object != Universe::None)
        {
            std::cout << "This is a class object";
            if (object->get_klass() && object->get_klass()->get_name())
                std::cout << " name is :" << object->get_klass()->get_name();
        }
        else
        {
            std::cout << "None";
        }
    }

}