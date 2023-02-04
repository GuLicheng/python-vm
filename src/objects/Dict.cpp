#include "Dict.hpp"
#include "String.hpp"
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
		return iter == this->dict.end() ? Universe::HiNone : iter->second;
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
			return Universe::HiNone;
		auto value = iter->second;
		this->dict.erase(iter);
		return value;
	}

    DictKlass::DictKlass()
    {
		this->set_name(new String("dict"));
    }

    Object* DictKlass::to_string(Object* x)
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