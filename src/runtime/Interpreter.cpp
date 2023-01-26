#include "Interpreter.hpp"
#include "../objects/List.hpp"
#include "../reader/BinaryFileParser.hpp"
#include "../objects/ByteCode.hpp"
#include "../objects/FunctionObject.hpp"
#include "../objects/Integer.hpp"
#include "../objects/Universe.hpp"
#include "../Python.hpp"
#include "../objects/NativeFunction.hpp"

#include <algorithm>

namespace python
{

    void Interpreter::push(Object* x)
	{
		this->frame->stack->add(x);
	}

	Object* Interpreter::pop()
	{
		return this->frame->stack->pop();
	}

	std::pair<Object*, Object*> Interpreter::pop_top_two()
	{
		auto w = this->frame->stack->pop();
		auto v = this->frame->stack->pop();
		return { w, v };
	}

	int Interpreter::stack_level() const
	{
		return this->frame->stack->size();
	}

	void Interpreter::build_frame(Object* callable, List* args, int op_arg)
	{
		if (callable->get_klass() == NativeFunctionKlass::get_instance())
		{
			auto ret = ((FunctionObject*)callable)->call(args);
			this->push(ret);
		}
		else if (callable->get_klass() == MemberFunctionKlass::get_instance())
		{
			auto method = callable->as<MemberFunctionObject>();
			if (!args)
			{
				args = new List(1);
			}
			args->insert(0, method->owner);
			this->build_frame(method->func, args, op_arg + 1);
		}
		else if (callable->get_klass() == FunctionKlass::get_instance())
		{
			FrameObject* fo = new FrameObject((FunctionObject*)callable, args, op_arg);

			// Push front f to linklist
			fo->sender = this->frame;
			this->frame = fo;
		}
		else
		{
			PYTHON_ASSERT(false && "Error function type");
		}
	}

	void Interpreter::eval_frame()
	{
		while (this->frame->has_more_codes())
		{
			unsigned char op_code = this->frame->get_op_code();
			bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;
			int op_arg = -1;

			if (has_argument)
			{
				op_arg = this->frame->get_op_arg();
			}

			switch (op_code)
			{
			case ByteCode::LOAD_CONST:
			{
				this->push(this->frame->co_consts->get(op_arg));
				break;
			}
			case ByteCode::PRINT_ITEM:
			{
				auto v = this->pop();
				v->print();
				break;
			}
			case ByteCode::PRINT_NEWLINE:
			{
				std::cout << '\n';
				break;
			}
			case ByteCode::BINARY_ADD:
			{
				auto [v, w] = this->pop_top_two();
				this->push(w->add(v));
				break;
			}
			case ByteCode::BINARY_SUBTRACT:
			{
				auto [v, w] = this->pop_top_two();
				this->push(w->sub(v));
				break;
			}
			case ByteCode::BINARY_DIVIDE:
			{
				auto [v, w] = this->pop_top_two();
				this->push(w->div(v));
				break;
			}
			case ByteCode::BINARY_MULTIPLY:
			{
				auto [v, w] = this->pop_top_two();
				this->push(w->mul(v));
				break;
			}
			case ByteCode::BINARY_MODULO:
			{
				auto [v, w] = this->pop_top_two();
				this->push(w->mod(v));
				break;
			}
			case ByteCode::RETURN_VALUE:
			{
				this->ret_value = this->pop();
				if (this->frame->is_first_frame())
					return;
				this->leave_frame();
				break;
			}
			case ByteCode::COMPARE_OP:
			{
				auto [rhs, lhs] = this->pop_top_two();
				switch (op_arg)
				{
				using enum ByteCode::COMPARE;
				case IS:
					this->push((lhs == rhs ? Universe::HiTrue : Universe::HiFalse));
					break;
				case IS_NOT:
					this->push((lhs != rhs ? Universe::HiTrue : Universe::HiFalse));
					break;
				case GREATER:
					this->push(lhs->greater(rhs));
					break;
				case LESS:
					this->push(lhs->less(rhs));
					break;
				case EQUAL:
					this->push(lhs->equal(rhs));
					break;
				case NOT_EQUAL:
					this->push(lhs->not_equal(rhs));
					break;
				case LESS_EQUAL:
					this->push(lhs->less_equal(rhs));
					break;
				case GREATER_EQUAL:
					this->push(lhs->greater_equal(rhs));
					break;
				default: std::unreachable();
				}
				break;
			}
			case ByteCode::POP_JUMP_IF_FALSE:
			{
				auto v = pop();
				if (((Integer*)v)->value() == 0)
					this->frame->pc = op_arg;
				break;
			}
			case ByteCode::JUMP_FORWARD:
			{
				this->frame->pc += op_arg;
				break;
			}
			case ByteCode::JUMP_ABSOLUTE:
			{
				this->frame->pc = op_arg;
				break;
			}
			case ByteCode::STORE_NAME:
			{
				auto v = this->frame->co_names->get(op_arg);
				auto t = this->pop();
				this->frame->locals->put(v, t);
				break;
			}
			case ByteCode::LOAD_NAME:
			{
				auto v = this->frame->co_names->get(op_arg);
				auto w = this->frame->locals->get(v);
				if (w != Universe::HiNone)
				{
					this->push(w);
					break;
				}

				w = this->frame->globals->get(v);
				if (w != Universe::HiNone)
				{
					this->push(w);
					break;
				}

				w = this->buildin->get(v);
				if (w != Universe::HiNone)
				{
					this->push(w);
					break;
				}

				this->push(Universe::HiNone);
				break;
			}
			case ByteCode::SETUP_LOOP:
			{
				this->frame->loop_stack->add(new Block{
					.type = op_code,
					.target = this->frame->pc + op_arg,
					.level = this->stack_level()
					});
				break;
			}
			case ByteCode::POP_BLOCK:
			{
				auto b = this->frame->loop_stack->pop();
				while (this->stack_level() > b->level)
				{
					this->pop();
				}
				break;
			}
			case ByteCode::BREAK_LOOP:
			{
				auto b = this->frame->loop_stack->pop();
				while (this->stack_level() > b->level)
				{
					this->pop();
				}
				this->frame->pc = b->target;
				break;
			}
			case ByteCode::MAKE_CLOSURE:
			{
				auto v = this->pop();
				auto fo = new FunctionObject(v);
				fo->closure = this->pop()->as<List>();
				fo->globals = this->frame->globals;
				List* args = nullptr;
				if (op_arg > 0)
				{
					args = new List();
					while (op_arg--)
					{
						args->append(this->pop());
					}
					args->reverse();
				}
				fo->defaults = args;
				this->push(fo);
				break;
			}
			case ByteCode::MAKE_FUNCTION:
			{
				auto v = this->pop();
				auto fo = new FunctionObject(v);
				fo->globals = this->frame->globals;

				List* args = nullptr;

				if (op_arg > 0)
				{
					args = new List(op_arg);
					while (op_arg--)
					{
						args->append(this->pop());
					}
					args->reverse();
					fo->defaults = args;
				}
				this->push(fo);
				break;
			}
			case ByteCode::CALL_FUNCTION:
			{
				List* args = nullptr;
				if (op_arg > 0)
				{
					int na = op_arg & 0xFF;
					int nk = op_arg >> 8;    // key params, times 2 since it requires name and value
					int arg_cnt = na + 2 * nk;
					args = new List(arg_cnt);
					while (arg_cnt--)
					{
						args->append(this->pop());
					}
					args->reverse();
				}
				this->build_frame(this->pop(), args, op_arg);
				break;
			}
			case ByteCode::LOAD_FAST:
			{
				this->push(this->frame->fast_local->get(op_arg));
				break;
			}
			case ByteCode::STORE_FAST:
			{
				if (op_arg >= this->frame->fast_local->size())
				{
					while (op_arg >= this->frame->fast_local->size())
					{
						this->frame->fast_local->append(nullptr);
					}
				}
				this->frame->fast_local->set(op_arg, this->pop());
				break;
			}
			case ByteCode::LOAD_GLOBAL:
			{
				auto v = this->frame->co_names->get(op_arg);
				auto w = this->frame->globals->get(v);
				if (w != Universe::HiNone) {
					this->push(w);
					break;
				}

				w = this->buildin->get(v);
				if (w != Universe::HiNone) {
					this->push(w);
					break;
				}

				this->push(Universe::HiNone);
				break;
			}
			case ByteCode::BINARY_SUBSCR:
			{
				auto [rhs, lhs] = this->pop_top_two();
				this->push(lhs->subscr(rhs));
				break;
			}
			case ByteCode::LOAD_ATTR:
			{
				auto v = this->pop();
				auto w = this->frame->co_names->get(op_arg);
				auto attr = v->getattr(w);
				this->push(attr);
				break;
			}
			case ByteCode::POP_TOP:
			{
				this->pop();
				break;
			}
			case ByteCode::STORE_DEREF:
			{
				this->frame->closure->set(op_arg, this->pop());
				break;
			}
			case ByteCode::LOAD_DEREF:
			{
                auto v = this->frame->closure->get(op_arg);
                if (v->is<CellObject>()) 
				{
                    v = v->as<CellObject>()->value();
                }
                this->push(v);
				break;
			}
			case ByteCode::LOAD_CLOSURE:
			{
				auto v = this->frame->closure->get(op_arg);
				if (!v)
				{
					v = this->frame->get_cell_from_parameter(op_arg);
					this->frame->closure->set(op_arg, v);
				}
				if (v->is<CellObject>())
				{
					this->push(v);
				}
				else
				{
					this->push(new CellObject(this->frame->closure, op_arg));
				}
				break;
			}
			case ByteCode::BUILD_TUPLE:
			case ByteCode::BUILD_LIST:
			{
				auto v = new List();
				while (op_arg--)
				{
					v->as<List>()->append(this->pop());
				}
				v->reverse();
				this->push(v);
				break;
			}
			case ByteCode::UNPACK_SEQUENCE:
			{
				auto v = this->pop();
				while (op_arg--)
				{
					this->push(v->subscr(new Integer(op_arg)));
				}
				break;
			}
			case ByteCode::STORE_GLOBAL:
			{
                auto v = this->frame->co_names->get(op_arg);
                this->frame->globals->put(v, this->pop());
                break;
			}
			default:
				std::cout << "Error byte: " << (int)op_code << "\n";
				std::terminate();
			}
		}

	}

	void Interpreter::destroy_frame()
	{
		FrameObject* fo = this->frame;
		this->frame = this->frame->sender;
		delete fo;
	}

	void Interpreter::leave_frame()
	{
		this->destroy_frame();
		this->push(this->ret_value);
	}

	Interpreter::Interpreter()
	{
		this->frame = nullptr;
		this->ret_value = nullptr;

		this->buildin = new Dict();

		this->buildin->put(new String("True"), Universe::HiTrue);
		this->buildin->put(new String("False"), Universe::HiFalse);
		this->buildin->put(new String("None"), Universe::HiNone);

		this->buildin->put(new String("len"), new FunctionObject(native::len));

		// TESTING:
	}

	void Interpreter::run(CodeObject* codes)
	{
		this->frame = new FrameObject(codes);

		this->eval_frame();
		this->destroy_frame();

	}

}