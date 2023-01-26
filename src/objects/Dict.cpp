#include "Dict.hpp"
#include "String.hpp"

namespace python
{
	Dict::Dict()
	{
		this->dict = new PythonObjectDictionary();
		this->klass = DictKlass::get_instance();
	}

	Dict::Dict(PythonObjectDictionary* d)
	{
		this->dict = d;
		this->klass = DictKlass::get_instance();
	}

	void Dict::put(Object* key, Object* value)
	{
		this->dict->insert_or_assign(key, value);
	}

	Object* Dict::get(Object* key)
	{
		auto iter = this->dict->find(key);
		return iter == this->dict->end() ? Universe::HiNone : iter->second;
	}

	bool Dict::has_key(Object* key)
	{
		return this->dict->contains(key);
	}

	int Dict::size() const
	{
		return this->dict->size();
	}

	Object* Dict::remove(Object* k)
	{
		auto iter = this->dict->find(k);
		if (iter == this->dict->end())
			return Universe::HiNone;
		auto value = iter->second;
		this->dict->erase(iter);
		return value;
	}

    DictKlass::DictKlass()
    {
		this->set_name(new String("dict"));
    }

}