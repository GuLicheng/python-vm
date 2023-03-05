#pragma once

#include "Object.hpp"

#include <iterator>

namespace python
{
    template <typename TIterator>
    class PyIteratorKlass : public Klass
    {
    public:

		virtual Object* __iter__(Object* x) override
        {
            return x;
        }
		
		virtual Object* __next__(Object* x) override
        {
            auto val = x->as<TIterator>()->value();
            x->as<TIterator>()->increase();
            return val;
        }

    };

	template <typename TObject, typename TIterator, typename TSentinel = TIterator>
	struct PyIterator : public Object
	{

		Object* iterable;   // store object and make it reachable for GC

		TIterator iterator;  // current
 
		TSentinel sentinel;  // end of iterator

        PyIterator(TObject* object)
        {
            this->iterable = object;
            auto [first, last] = object->get_iterator_pair();
            this->iterator = first;
            this->sentinel = last;
        }

		void increase() 
        {
            // Some container may not support ++end() e.g. std::unordered_map 
            std::ranges::advance(this->iterator, 1, this->sentinel); 
        }

		bool is_over() const 
        { 
            return this->iterator == this->sentinel; 
        }

		Object* value() 
        { 
            return is_over() ? nullptr : *(this->iterator); 
        }

	};
}

