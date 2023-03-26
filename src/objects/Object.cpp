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
        this->m_obj_dict = new Dict();
    }

    void Object::merge_obj_dict(Object* object)
    {
        auto dict = object->m_obj_dict; 
        if (!this->m_obj_dict)
        {
            this->m_obj_dict = dict;
        }
        else
        {
            this->m_obj_dict->value().merge(dict->value());
        }
    }

    void Object::mark()
    {
        this->m_mark = true;
    }

    void Object::mark_self_and_children()
    {
        this->m_klass->mark_self_and_children(this);
    }

    void Object::unmark()
    {
        this->m_mark = false;
    }

    bool Object::is_marked()
    {
        return this->m_mark;
    }

    void Object::print()
    {
        // std::cout << "name = " << this->m_klass->get_name()->value() << '\n';
        this->m_klass->print(this);
    }

    Object* Object::py__add__(Object* self)
    {
        return this->m_klass->py__add__(this, self);
    }

    Object* Object::py__sub__(Object* self)
    {
        return this->m_klass->py__sub__(this, self);
    }

    Object* Object::py__mul__(Object* self)
    {
        return this->m_klass->py__mul__(this, self);
    }

    Object* Object::py__div__(Object* self)
    {
        return this->m_klass->py__div__(this, self);
    }

    Object* Object::py__floordiv__(Object* self)
    {
        return this->m_klass->py__floordiv__(this, self);
    }

    Object* Object::py__mod__(Object* self)
    {
        return this->m_klass->py__mod__(this, self);
    }

    Object* Object::py__gt__(Object* self)
    {
        return this->m_klass->py__gt__(this, self);
    }

    Object* Object::py__lt__(Object* self)
    {
        return this->m_klass->py__lt__(this, self);
    }

    Object* Object::py__eq__(Object* self)
    {
        return this->m_klass->py__eq__(this, self);
    }

    Object* Object::py__ne__(Object* self)
    {
        return this->m_klass->py__ne__(this, self);
    }

    Object* Object::py__le__(Object* self)
    {
        return this->m_klass->py__le__(this, self);
    }

    Object* Object::py__ge__(Object* self)
    {
        return this->m_klass->py__ge__(this, self);
    }

    Object* Object::py__contains__(Object* self)
    {
        return this->m_klass->py__contains__(this, self);
    }

    Object* Object::py__hash__()
    {
        return this->m_klass->py__hash__(this);
    }

    Object* Object::py__deepcopy__()
    {
        return this->m_klass->py__deepcopy__(this);
    }

    Object* Object::py__len__()
    {
        return this->m_klass->py__len__(this);
    }

    Object* Object::py__iter__()
    {
        return this->m_klass->py__iter__(this);
    }

    Object* Object::py__next__()
    {
        return this->m_klass->py__next__(this);
    }

    Object* Object::py__str__()
    {
        return this->m_klass->py__str__(this);
    }

    Object* Object::py__int__()
    {
        return this->m_klass->py__int__(this);
    }

    Object* Object::py__float__()
    {
        return this->m_klass->py__float__(this);
    }

    Object* Object::py__bool__()
    {
        return this->m_klass->py__bool__(this);
    }

    Object* Object::py__getattr__(Object* attribute)
    {
        return this->m_klass->py__getattr__(this, attribute);
    }

    void Object::py__setitem__(Object* key, Object* value)
    {
        this->m_klass->py__setitem__(this, key, value);
    }

    Object* Object::py__getitem__(Object* name)
    {
        return this->m_klass->py__getitem__(this, name);
    }

    Object* Object::get_klass_attr(Object* attribute)
    {
        return this->m_klass->get_klass_attr(this, attribute);
    }

    Object* Object::py__setattr__(Object* key, Object* value)
    {
        return this->m_klass->py__setattr__(this, key, value);
    }
    
    void ObjectKlass::print(Object* self)
    {
        if (self != Universe::None)
        {
            std::cout << "This is a class self";
            if (self->get_klass() && self->get_klass()->get_name())
                std::cout << " name is :" << self->get_klass()->get_name();
        }
        else
        {
            std::cout << "None";
        }
    }

}