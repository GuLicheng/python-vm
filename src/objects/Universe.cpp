#include "Universe.hpp"
#include "Integer.hpp"
#include "Float.hpp"
#include "Object.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "String.hpp"
#include "FunctionObject.hpp"
#include "Klass.hpp"
#include "TypeObject.hpp"
#include "../runtime/Interpreter.hpp"

#include <vector>

namespace python
{
    void Universe::register_klass(Klass* klass)
    {
        klasses.emplace_back(klass);
    }

    void Universe::genesis()
    {
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

        StringKlass::get_instance()->initialize();
        ListKlass::get_instance()->initialize();
        DictKlass::get_instance()->initialize();

        
        type_klass->set_klass_dict(new Dict());
        object_klass->set_klass_dict(new Dict());

        type_klass->set_name(new String("type"));
        object_klass->set_name(new String("object"));

        Interpreter::get_instance()->initialize();

    }

    void Universe::destroy()
    {
    }

}

