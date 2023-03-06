#include "Universe.hpp"
#include "Integer.hpp"
#include "Double.hpp"
#include "Object.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "String.hpp"
#include "FunctionObject.hpp"
#include "Klass.hpp"
#include "TypeObject.hpp"
#include "../runtime/Interpreter.hpp"

namespace python
{
	void Universe::genesis()
	{

		klasses = new List();

		True = new Integer(1);
		False = new Integer(0);
		None = new Object();

		// Initialize type object
		Klass* object_klass = ObjectKlass::get_instance();
		Klass* type_klass   = TypeKlass::get_instance();

		TypeObject* tp_obj = new TypeObject();
		tp_obj->set_own_klass(type_klass);

		TypeObject* obj_obj = new TypeObject();
		obj_obj->set_own_klass(object_klass);

		type_klass->add_super(object_klass);
		// do nothing for object klass
		//object_klass->add_super(NULL);

		IntegerKlass::get_instance()->initialize();
		DoubleKlass::get_instance()->initialize();
		StringKlass::get_instance()->initialize();
		ListKlass::get_instance()->initialize();
		DictKlass::get_instance()->initialize();
		// ModuleKlass::get_instance()->initialize();

		FunctionKlass::get_instance()->initialize();
		NativeFunctionKlass::get_instance()->initialize();
		MemberFunctionKlass::get_instance()->initialize();
		CellKlass::get_instance()->initialize();

		
		type_klass->set_klass_dict(new Dict());
		object_klass->set_klass_dict(new Dict());

		type_klass->set_name(new String("type"));
		object_klass->set_name(new String("object"));

		IntegerKlass::get_instance()->order_supers();
		DoubleKlass::get_instance()->order_supers();
		StringKlass::get_instance()->order_supers();
		DictKlass::get_instance()->order_supers();
		ListKlass::get_instance()->order_supers();

		type_klass->order_supers();

		FunctionKlass::get_instance()->order_supers();
		NativeFunctionKlass::get_instance()->order_supers();
		MemberFunctionKlass::get_instance()->order_supers();


		Interpreter::get_instance()->initialize();

		// std::cout << "ObjectKlass " << ObjectKlass::get_instance() << '\n';
		// std::cout << "TypeKlass " << TypeKlass::get_instance() << '\n';
		
		// std::cout << "IntegerKlass " << IntegerKlass::get_instance() << '\n';
		// std::cout << "DoubleKlass " << DoubleKlass::get_instance() << '\n';
		// std::cout << "StringKlass " << StringKlass::get_instance() << '\n';
		// std::cout << "DictKlass " << DictKlass::get_instance() << '\n';
		// std::cout << "ListKlass " << ListKlass::get_instance() << '\n';

	}

	void Universe::destroy()
	{
	}

}

