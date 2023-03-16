#include "ModuleObject.hpp"
#include "Dict.hpp"
#include "String.hpp"

namespace python
{
    void ModuleKlass::initialize()
    {
        this->build_klass("module", ObjectKlass::get_instance(), new Dict());
    }

    size_t ModuleKlass::size()
    {
        return sizeof(ModuleObject);
    }

    ModuleObject::ModuleObject(Dict* x)
    {
        this->m_obj_dict = x;
        this->set_klass(ModuleKlass::get_instance());
    }

    void ModuleObject::put(Object* key, Object* value)
    {
        this->m_obj_dict->put(key, value);
    }

    Object* ModuleObject::get(Object* key)
    {
        return this->m_obj_dict->get(key);
    }

    void ModuleObject::extend(ModuleObject* module_)
    {
        // this->obj_dict->d
    }

}
