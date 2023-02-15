#pragma once

#include "../Objects/CodeObject.hpp"
#include "../ArrayList.hpp"
#include "../Objects/Object.hpp"
#include "../Objects/FrameObject.hpp"
#include "../Objects/Dict.hpp"

#include <utility>


namespace python
{

	class Interpreter : public Singleton<Interpreter>
	{

		FrameObject* frame;

		Object* ret_value;

		Dict* buildin;

	private:
		void push(Object* x);

		Object* pop();

		std::pair<Object*, Object*> pop_top_two();

		int stack_level() const;

		void build_frame(Object* callable, List* args, int op_arg);

		void eval_frame();

		void leave_frame();

		void destroy_frame();

	public:

		Interpreter();

		Object* call_virtual(Object* func, List* args) { NOT_IMPLEMENT; }

		void run(CodeObject* codes);

	};
}



