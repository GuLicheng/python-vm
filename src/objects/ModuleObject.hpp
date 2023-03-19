#pragma once

#include "Klass.hpp"
#include "Singleton.hpp"
#include "Object.hpp"

namespace python
{
    
    class ModuleKlass : public Klass, public Singleton<ModuleKlass>
    {
    public:

        void initialize();

        virtual size_t size() override;
    };

    class ModuleObject : public Object
    {
        friend class ModuleKlass;

        class String* m_module_name;

    public:

        using KlassType = ModuleKlass;

        ModuleObject(class Dict* x);

        // static ModuleObject* import_module(Object* mod_name);
        
        void put(Object* key, Object* value);

        Object* get(Object* key);

        void extend(ModuleObject* module_obj);
    };

} // namespace python

