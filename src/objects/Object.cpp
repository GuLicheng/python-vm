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

	Object* Object::add(Object* x)
	{
		return this->klass->add(this, x);
	}

	Object* Object::__sub__(Object* x)
	{
		return this->klass->__sub__(this, x);
	}

	Object* Object::__mul__(Object* x)
	{
		return this->klass->__mul__(this, x);
	}

	Object* Object::div(Object* x)
	{
		return this->klass->div(this, x);
	}

	Object* Object::mod(Object* x)
	{
		return this->klass->mod(this, x);
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

	Object* Object::subscr(Object* x)
	{
		return this->klass->subscr(this, x);
	}

	Object* Object::contains(Object* x)
	{
		return this->klass->contains(this, x);
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

    Object* Object::to_string()
    {
        return this->klass->to_string(this);
    }

    Object* Object::getattr(Object* attribute)
    {
        return this->klass->getattr(this, attribute);
    }

    Object* Object::get_klass_attr(Object* attribute)
    {
        return this->klass->get_klass_attr(this, attribute);
    }

    Object* Object::setattr(Object* key, Object* value)
    {
        return this->klass->setattr(this, key, value);
    }
    
    void ObjectKlass::print(Object* object)
    {
		if (object != Universe::None)
			std::cout << "This is a class object";
		else
			std::cout << "None";
    }

}