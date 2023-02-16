#include "Universe.hpp"
#include "Integer.hpp"
#include "Double.hpp"
#include "Object.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "String.hpp"
#include "NativeFunction.hpp"
#include "FunctionObject.hpp"
#include "Klass.hpp"
#include "TypeObject.hpp"

namespace python
{
	void Universe::genesis()
	{
		True = new Integer(1);
		False = new Integer(0);
		None = new Object();

		// Initialize
		// Klass* object_klass = ObjectKlass::get_instance();
		// Klass* type_klass = TypeKlass::get_instance();

		IntegerKlass::get_instance()->initialize();
		DoubleKlass::get_instance()->initialize();
		StringKlass::get_instance()->initialize();
		ListKlass::get_instance()->initialize();
		DictKlass::get_instance()->initialize();
		// ModuleKlass::get_instance()->initialize();


	}

	void Universe::destroy()
	{
	}

}

