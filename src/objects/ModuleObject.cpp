#include "ModuleObject.hpp"
#include "Dict.hpp"
#include "String.hpp"

namespace python
{
    void ModuleKlass::initialize()
    {
        Dict* dict = new Dict();
        this->set_klass_dict(dict);
        this->set_name(new String("module"));
        (new Object())->set_klass(this);
        this->add_super(ObjectKlass::get_instance());
    }

    size_t ModuleKlass::size()
    {
        return sizeof(ModuleObject);
    }

    ModuleObject::ModuleObject(Dict* x)
    {
        this->obj_dict = x;
        this->set_klass(ModuleKlass::get_instance());
    }

    void ModuleObject::put(Object* key, Object* value)
    {
        this->obj_dict->put(key, value);
    }

    Object* ModuleObject::get(Object* key)
    {
        return this->obj_dict->get(key);
    }

    void ModuleObject::extend(ModuleObject* module_)
    {
        // this->obj_dict->d
    }

}
