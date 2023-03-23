#include "Super.hpp"
#include "Dict.hpp"
#include "List.hpp"
#include "TypeObject.hpp"
#include "FunctionObject.hpp"
#include "StringTable.hpp"

namespace python
{
 

    /*
        class Derived(Base):
            def __init__(self)
                super(Derived, self).__init__()
    */
    SuperKlass::SuperKlass()
    {
        auto dict = new Dict();
        auto ctor = new FunctionObject(
            [](List* args) -> Object* { 
                auto super = args->get(0)->as<SuperObject>();
                auto first = super->m_type->get_own_klass()->m_klass_mro.front();
                PYTHON_ASSERT(first != ObjectKlass::get_instance());
                List* ls = nullptr;
                if (args->size() > 1)
                {
                    ls = new List();
                    for (int i = 1; i < args->size(); ++i)
                        ls->append(args->get(i));
                }
                auto base = first->allocate_instance(first->get_type_object(), ls);
                base->set_klass(super->m_type->get_own_klass());
                super->m_instance->merge_obj_dict(base);
                return base;
            }
        );

        dict->put(StringTable::init, ctor);  
        this->build_klass("super", ObjectKlass::get_instance(), dict);
    }

    Object* SuperKlass::allocate_instance(Object* callable, List* args)
    {
        native::detail::check_argument_count(args, 2);
        auto type = args->get(0);
        auto instance = args->get(1);
        PYTHON_ASSERT(type->expected<TypeKlass>());
        return new SuperObject(type->as<TypeObject>(), instance);
    }

    SuperObject::SuperObject(TypeObject* type, Object* instance)
        : m_type(type), m_instance(instance)
    {
        this->set_klass(SuperKlass::get_instance());
    }
} // namespace python