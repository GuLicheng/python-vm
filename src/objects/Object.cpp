#include "Object.hpp"
#include "Universe.hpp"
#include "Dict.hpp"
#include "FunctionObject.hpp"

namespace python
{
	void Object::print()
	{
		this->klass->print(this);
	}

	Object* Object::add(Object* x)
	{
		return this->klass->add(this, x);
	}

	Object* Object::sub(Object* x)
	{
		return this->klass->sub(this, x);
	}

	Object* Object::mul(Object* x)
	{
		return this->klass->mul(this, x);
	}

	Object* Object::div(Object* x)
	{
		return this->klass->div(this, x);
	}

	Object* Object::mod(Object* x)
	{
		return this->klass->mod(this, x);
	}

	Object* Object::greater(Object* x)
	{
		return this->klass->greater(this, x);
	}

	Object* Object::less(Object* x)
	{
		return this->klass->less(this, x);
	}

	Object* Object::equal(Object* x)
	{
		return this->klass->equal(this, x);
	}

	Object* Object::not_equal(Object* x)
	{
		return this->klass->not_equal(this, x);
	}

	Object* Object::less_equal(Object* x)
	{
		return this->klass->less_equal(this, x);
	}

	Object* Object::greater_equal(Object* x)
	{
		return this->klass->greater_equal(this, x);
	}

	Object* Object::subscr(Object* x)
	{
		return this->klass->subscr(this, x);
	}

	Object* Object::contains(Object* x)
	{
		return this->klass->contains(this, x);
	}

	Object* Object::hash_code()
	{
		return this->klass->hash_code(this);
	}

	Object* Object::deepcopy()
	{
		return this->klass->deepcopy(this);
	}

	Object* Object::length()
	{
		return this->klass->length(this);
	}

    Object* Object::to_string()
    {
        return this->klass->to_string(this);
    }

    Object* Object::getattr(Object* x)
	{
		auto result = x->klass->get_klass_dict()->get(x);

		if (result == Universe::None)
		{
			return result;
		}

		// Only klass attr needs bind
		if (result->get_klass() == FunctionKlass::get_instance()
		  || result->get_klass() == NativeFunctionKlass::get_instance())
		{
			result = new MemberFunctionObject((FunctionObject*)result, this);
		}

		return result;
	}
}