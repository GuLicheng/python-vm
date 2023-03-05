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

	Object* Object::__add__(Object* x)
	{
		return this->klass->__add__(this, x);
	}

	Object* Object::__sub__(Object* x)
	{
		return this->klass->__sub__(this, x);
	}

	Object* Object::__mul__(Object* x)
	{
		return this->klass->__mul__(this, x);
	}

	Object* Object::__div__(Object* x)
	{
		return this->klass->__div__(this, x);
	}

    Object* Object::__floordiv__(Object* x)
    {
        return this->klass->__floordiv__(this, x);
    }

	Object* Object::__mod__(Object* x)
	{
		return this->klass->__mod__(this, x);
	}

	Object* Object::__gt__(Object* x)
	{
		return this->klass->__gt__(this, x);
	}

	Object* Object::__lt__(Object* x)
	{
		return this->klass->__lt__(this, x);
	}

	Object* Object::__eq__(Object* x)
	{
		return this->klass->__eq__(this, x);
	}

	Object* Object::__ne__(Object* x)
	{
		return this->klass->__ne__(this, x);
	}

	Object* Object::__le__(Object* x)
	{
		return this->klass->__le__(this, x);
	}

	Object* Object::__ge__(Object* x)
	{
		return this->klass->__ge__(this, x);
	}

	Object* Object::__contains__(Object* x)
	{
		return this->klass->__contains__(this, x);
	}

	Object* Object::__hash__()
	{
		return this->klass->__hash__(this);
	}

	Object* Object::__deepcopy__()
	{
		return this->klass->__deepcopy__(this);
	}

	Object* Object::__len__()
	{
		return this->klass->__len__(this);
	}

    Object* Object::__iter__()
    {
        return this->klass->__iter__(this);
    }

    Object* Object::__next__()
    {
        return this->klass->__next__(this);
    }

    Object* Object::__str__()
    {
        return this->klass->__str__(this);
    }

    Object* Object::__int__()
    {
        return this->klass->__int__(this);
    }

    Object* Object::__float__()
    {
        return this->klass->__float__(this);
    }

    Object* Object::__bool__()
    {
        return this->klass->__bool__(this);
    }

    Object* Object::__getattr__(Object* attribute)
    {
        return this->klass->__getattr__(this, attribute);
    }

    void Object::__setitem__(Object* key, Object* value)
    {
        this->klass->__setitem__(this, key, value);
    }

    Object* Object::__getitem__(Object* name)
	{
		return this->klass->__getitem__(this, name);
	}

    Object* Object::get_klass_attr(Object* attribute)
    {
        return this->klass->get_klass_attr(this, attribute);
    }

    Object* Object::__setattr__(Object* key, Object* value)
    {
        return this->klass->__setattr__(this, key, value);
    }
    
    void ObjectKlass::print(Object* object)
    {
		if (object != Universe::None)
			std::cout << "This is a class object";
		else
			std::cout << "None";
    }

}