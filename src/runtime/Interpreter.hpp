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
		enum struct Status
		{
			IS_OK,
			IS_BREAK,
			IS_CONTINUE,
			IS_EXCEPTION,
			IS_RETURN,
			IS_YIELD,
		};

		FrameObject* frame;

		Object* ret_value;

		Dict* buildin;

		Status status;

	private:
		void push(Object* x);

		Object* pop();

		std::pair<Object*, Object*> pop_top_two();

		int stack_level() const;

		void build_frame(Object* callable, List* args, int op_arg);

		void eval_frame();

		void leave_frame();

		void destroy_frame();

		void enter_frame(FrameObject* frame);

	public:

		Interpreter();

		void initialize();

		Object* call_virtual(Object* func, List* args);

		void run(CodeObject* codes);

	};
}



