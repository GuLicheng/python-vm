#include "Dict.hpp"
#include "String.hpp"
#include "TypeObject.hpp"
#include "List.hpp"
#include "FunctionObject.hpp"
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

    DictKlass::DictKlass()
    {
    }

    void DictKlass::initialize()
    {
		Dict* klass_dict = new Dict();
		// klass_dict->put(new String("setdefault"),
		// 		new FunctionObject(dict_set_default));
		// klass_dict->put(new String("remove"),
		// 		new FunctionObject(dict_remove));
		// klass_dict->put(new String("keys"),
		// 		new FunctionObject(dict_keys));
		// klass_dict->put(new String("values"),
		// 		new FunctionObject(dict_values));
		// klass_dict->put(new String("items"),
		// 		new FunctionObject(dict_items));
		// klass_dict->put(new String("iterkeys"),
		// 		new FunctionObject(dict_iterkeys));
		// klass_dict->put(new String("itervalues"),
		// 		new FunctionObject(dict_itervalues));
		// klass_dict->put(new String("iteritems"),
		// 		new FunctionObject(dict_iteritems));
		this->set_klass_dict(klass_dict);
		this->set_name(new String("dict"));
		(new TypeObject)->set_own_klass(this);
		this->add_super(ObjectKlass::get_instance());
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

    Object *DictKlass::to_string(Object *x)
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
			ss << k->to_string()->as<String>()->value();
			ss << ",";
			ss << v->to_string()->as<String>()->value();
		}
		ss << '}';
		return new String(ss.str());
    }
}