#include "Dict.hpp"
#include "String.hpp"
#include "TypeObject.hpp"
#include "List.hpp"
#include "FunctionObject.hpp"
#include "Iterator.hpp"

#include <sstream>

namespace python
{
    Dict::Dict()
    {
        this->klass = DictKlass::get_instance();
    }

    Dict::Dict(PythonObjectDictionary d)
    {
        this->dict = std::move(d);
        this->klass = DictKlass::get_instance();
    }

    void Dict::put(Object* key, Object* value)
    {
        this->dict.insert_or_assign(key, value);
    }

    Object* Dict::get(Object* key)
    {
        auto iter = this->dict.find(key);
        return iter == this->dict.end() ? Universe::None : iter->second;
    }

    bool Dict::has_key(Object* key)
    {
        return this->dict.contains(key);
    }

    int Dict::size() const
    {
        return this->dict.size();
    }

    Object* Dict::remove(Object* k)
    {
        auto iter = this->dict.find(k);
        if (iter == this->dict.end())
            return Universe::None;
        auto value = iter->second;
        this->dict.erase(iter);
        return value;
    }

    PythonObjectDictionary& Dict::value()
    {
        return this->dict;
    }

    void DictKlass::initialize()
    {
        Dict* klass_dict = new Dict();
        klass_dict->put(new String("keys"), new FunctionObject(native::dict_keys2));
        klass_dict->put(new String("values"), new FunctionObject(native::dict_values2));
        klass_dict->put(new String("items"), new FunctionObject(native::dict_items2));
        // klass_dict->put(new String("setdefault"),
        //         new FunctionObject(dict_set_default));
        // klass_dict->put(new String("remove"),
        //         new FunctionObject(dict_remove));
        // klass_dict->put(new String("keys"),
        //         new FunctionObject(dict_keys));
        // klass_dict->put(new String("values"),
        //         new FunctionObject(dict_values));
        // klass_dict->put(new String("items"),
        //         new FunctionObject(dict_items));
        // klass_dict->put(new String("iterkeys"),
        //         new FunctionObject(dict_iterkeys));
        // klass_dict->put(new String("itervalues"),
        //         new FunctionObject(dict_itervalues));
        // klass_dict->put(new String("iteritems"),
        //         new FunctionObject(dict_iteritems));
        this->build_klass("dict", ObjectKlass::get_instance(), klass_dict);
    }

    Object* DictKlass::allocate_instance(Object* callable, List* args)
    {
        if (!args || args->size() == 0)
            return new Dict();
        else
            return nullptr;
    }

    size_t DictKlass::size()
    {
        return sizeof(Dict);
    }

    Object* DictKlass::__str__(Object* x)
    {
        PYTHON_ASSERT(x && x->is<Dict>());
        std::stringstream ss;
        bool first = true;
        ss << '{';
        for (auto& d = x->as<Dict>()->dict; auto& [k, v] : d)
        {
            if (!first)
                ss << ", ";
            first = false;
            ss << k->__str__()->as<String>()->value();
            ss << ",";
            ss << v->__str__()->as<String>()->value();
        }
        ss << '}';
        return new String(ss.str());
    }

    Object* DictKlass::__contains__(Object* x, Object* y)
    {
        PYTHON_ASSERT(x && x->is<Dict>());
        return x->as<Dict>()->has_key(y) ? Universe::True : Universe::False;
    }

    Object* DictKlass::__iter__(Object* x)
    {
        return (new PyView(x, x->as<Dict>()->dict | std::views::transform([](auto pair){
            List* ls = new List();
            ls->append(pair.first);
            ls->append(pair.second);
            return ls;
        }), "dict_iterator"))->get_iterator();
    }
    
    void DictKlass::print(Object* object)
    {
        auto dict = object->as<Dict>();
        for (auto [k, v] : dict->dict)
        {
            k->print();
            std::cout << ", ";
            v->print();
            std::cout << ' ';
        }
    }
}

namespace python::native
{
    Object* dict_keys2(List* args)
    {
        auto py_dict = detail::check_and_get_from_argument_list<Dict>(args, 0, 1);
        return new PyView(py_dict, py_dict->as<Dict>()->value() | std::views::keys, "dict_key");
    }

    Object* dict_values2(List* args)
    {
        auto py_dict = detail::check_and_get_from_argument_list<Dict>(args, 0, 1);
        return new PyView(py_dict, py_dict->as<Dict>()->value() | std::views::values, "dict_value");
    }

    Object* dict_items2(List* args)
    {
        auto py_dict = detail::check_and_get_from_argument_list<Dict>(args, 0, 1);
        return new PyView(py_dict, py_dict->as<Dict>()->value() | std::views::transform([](auto pair){
            List* ls = new List();
            ls->append(pair.first);
            ls->append(pair.second);
            return ls;
        }), "dict_view");
    }    
}


