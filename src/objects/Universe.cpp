#include "Universe.hpp"
#include "Integer.hpp"
#include "Object.hpp"
#include "Dict.hpp"
#include "String.hpp"
#include "NativeFunction.hpp"
#include "FunctionObject.hpp"

namespace python
{
	void Universe::genesis()
	{
		HiTrue = new Integer(1);
		HiFalse = new Integer(0);
		HiNone = new Object();

		Dict* klass_dict = new Dict();
		StringKlass::get_instance()->set_klass_dict(klass_dict);
		klass_dict->put(new String("upper"), new FunctionObject(native::string_upper));
	}

	void Universe::destroy()
	{
	}

}

