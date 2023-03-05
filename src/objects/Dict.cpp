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

    std::array<PythonObjectDictionary::iterator, 2> Dict::get_iterator_pair()
    {
        return { this->dict.begin(), this->dict.end() };
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
        return new DictIterator<DictIteratorMode::Items>(x->as<Dict>());
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

    template<DictIteratorMode EMode>
    inline DictIterator<EMode>::DictIterator(Dict* dict) : base(dict)
    {
		this->klass = DictIteratorKlass<EMode>::get_instance();
    }

    template<DictIteratorMode EMode>
    inline Object* DictIterator<EMode>::value()
	{
		if constexpr (EMode == DictIteratorMode::Items) 
		{
			if (this->is_over()) return nullptr;
			List* ls = new List();
			ls->append((*(this->iterator)).first);	
			ls->append((*(this->iterator)).second);
			return ls;	
		} 
		else if constexpr (EMode == DictIteratorMode::Keys)
		{
			return this->is_over() ? nullptr : (*(this->iterator)).first;
		}
		else
		{
			return this->is_over() ? nullptr : (*(this->iterator)).second;
		} 
	}

    template<DictIteratorMode EMode>
    inline DictIteratorKlass<EMode>::DictIteratorKlass()
    {
    }

    template<DictIteratorMode EMode>
    inline void DictIteratorKlass<EMode>::initialize()
    {
		Dict* dict = new Dict();
		
		this->klass_dict = dict;

		constexpr auto name = [](){
			if constexpr (EMode == DictIteratorMode::Items)
				return "dict_item_iterator";
			else if constexpr (EMode == DictIteratorMode::Keys)
				return "dict_key_iterator";
			else 
				return "dict_value_iterator";
		}();

		(new TypeObject)->set_own_klass(this);
		this->set_name(new String(name));
		this->add_super(ObjectKlass::get_instance());
    }

    template<DictIteratorMode EMode>
    void DictIteratorKlass<EMode>::print(Object* x)
    {
		constexpr auto info = [](){
			if constexpr (EMode == DictIteratorMode::Items)
				return "DictIteratorItems";
			else if constexpr (EMode == DictIteratorMode::Keys)
				return "DictIteratorKeys";
			else 
				return "DictIteratorValues";
		}();

		std::cout << info;
    }

	template class DictIterator<DictIteratorMode::Items>;
	
	template class DictIterator<DictIteratorMode::Keys>;

	template class DictIterator<DictIteratorMode::Values>;

	template class DictIteratorKlass<DictIteratorMode::Items>;

	template class DictIteratorKlass<DictIteratorMode::Keys>;
	
	template class DictIteratorKlass<DictIteratorMode::Values>;

}