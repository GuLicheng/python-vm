#include "Interpreter.hpp"
#include "../objects/List.hpp"
#include "../reader/BinaryFileParser.hpp"
#include "../objects/ByteCode.hpp"
#include "../objects/FunctionObject.hpp"
#include "../objects/Generator.hpp"
#include "../objects/Integer.hpp"
#include "../objects/Universe.hpp"
#include "../Python.hpp"
#include "../objects/NativeFunction.hpp"
#include "../objects/StringTable.hpp"
#include "../objects/Integer.hpp"
#include "../objects/List.hpp"
#include "../objects/String.hpp"
#include "../objects/Dict.hpp"
#include "../objects/Object.hpp"
#include "../objects/TypeObject.hpp"
#include "../objects/NativeClass.hpp"
#include "Traceback.hpp"

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

    Object* Interpreter::top()
    {
        return this->frame->stack->get(-1);
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
            auto ret = callable->as<FunctionObject>()->call(args);
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
        else if (callable->get_klass() == TypeKlass::get_instance())
        {
            auto own_class = callable->as<TypeObject>()->get_own_klass();
            Object* instance = own_class->allocate_instance(callable, args);
            this->push(instance);
        }
        else if (MemberFunctionObject::is_yield_function(callable))
        {
            auto generator = new Generator(callable->as<FunctionObject>(), args, op_arg);
            this->push(generator);
            return;
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
            // PYTHON_ASSERT(false && "Error function type");
            Object* m = callable->get_klass_attr(StringTable::call);
            if (m != Universe::None)
            {
                this->build_frame(m, args, op_arg);
            }
            else
            {
                callable->print();
                if (callable != Universe::None)
                    PYTHON_ASSERT(false && "cannot call a normal object.");
                else
                    PYTHON_ASSERT(false && "None object is not callable.");
            }
        }
    }

    void Interpreter::eval_frame()
    {

        while (this->frame->has_more_codes())
        {
            // PYTHON_ASSERT(this->status == Status::IS_OK);

            unsigned char op_code = this->frame->get_op_code();
            bool has_argument = (op_code & 0xFF) >= ByteCode::HAVE_ARGUMENT;
            int op_arg = -1;

            if (has_argument)
            {
                op_arg = this->frame->get_op_arg();
            }

            // std::cout << "Evaluating: " << (int)op_code << '\n';

            switch (op_code)
            {
            case ByteCode::YIELD_VALUE:
            {
                this->status = Status::IS_YIELD;
                this->ret_value = this->top();
                return;  // Directory return for `yield`
            }
            case ByteCode::STORE_MAP:
            {
                auto w = this->pop();
                auto u = this->pop();
                auto v = this->top();
                v->as<Dict>()->put(w, u);
                break;
            }
            case ByteCode::BUILD_MAP:
            {
                this->push(new Dict());
                break;
            }
            case ByteCode::GET_ITER:
            {
                auto v = this->pop();
                this->push(v->py__iter__());
                break;
            }
            case ByteCode::FOR_ITER:
            {
                // For-loop in Python will call GET_ITER and FOR_ITER.
                // for follow code:
                // >>> for val in [1, 2, 3]:
                // >>>   print(val)
                // will generate byte code like:
                //         SET_LOOP
                //         ... 
                //         GET_ITER
                //         FOR_ITER
                //         LOAD_NAME   
                // If v is list, v.py__iter__() should return it's iterator.
                // If v is iterator, v.py__iter__() should return itself.       
                auto v = this->top();
                auto w = v->py__next__();
                if (!w)
                {
                    // We throw StopIteration in `py__next__` or `next`.
                    this->frame->pc += op_arg;
                    this->status = Status::IS_OK;
                    this->pending_exception = nullptr;
                }
                else
                {
                    this->push(w);
                }
                break;
            }
            case ByteCode::DUP_TOP:
            {
                auto v = this->pop();
                this->push(v);
                this->push(v);
                break;
            }
            case ByteCode::BUILD_CLASS:
            {
                auto v = this->pop();
                auto supers = this->pop();
                auto name = this->pop();
                v = Klass::create_klass(v, supers, name);
                this->push(v);
                break;
            }
            case ByteCode::LOAD_LOCALS:
            {
                this->push(this->frame->locals);
                break;
            }
            case ByteCode::LOAD_CONST:
            {
                auto c = this->frame->co_consts->get(op_arg);
                this->push(c);
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
            case ByteCode::INPLACE_ADD:
            case ByteCode::BINARY_ADD:
            {
                auto [v, w] = this->pop_top_two();
                this->push(w->py__add__(v));
                break;
            }
            case ByteCode::INPLACE_SUBSTRACT:
            case ByteCode::BINARY_SUBTRACT:
            {
                auto [v, w] = this->pop_top_two();
                this->push(w->py__sub__(v));
                break;
            }
            case ByteCode::BINARY_DIVIDE:
            {
                auto [v, w] = this->pop_top_two();
                this->push(w->py__div__(v));
                break;
            }
            case ByteCode::BINARY_MULTIPLY:
            {
                auto [v, w] = this->pop_top_two();
                this->push(w->py__mul__(v));
                break;
            }
            case ByteCode::BINARY_MODULO:
            {
                auto [v, w] = this->pop_top_two();
                this->push(w->py__mod__(v));
                break;
            }
            case ByteCode::RETURN_VALUE:
            {
                this->ret_value = this->pop();
                this->status = Status::IS_RETURN;
                break;
            }
            case ByteCode::COMPARE_OP:
            {
                auto [rhs, lhs] = this->pop_top_two();
                switch (op_arg)
                {
                using enum ByteCode::COMPARE;
                case IS:
                    this->push((lhs == rhs ? Universe::True : Universe::False));
                    break;
                case IS_NOT:
                    this->push((lhs != rhs ? Universe::True : Universe::False));
                    break;
                case GREATER:
                    this->push(lhs->py__gt__(rhs));
                    break;
                case LESS:
                    this->push(lhs->py__lt__(rhs));
                    break;
                case EQUAL:
                    this->push(lhs->py__eq__(rhs));
                    break;
                case NOT_EQUAL:
                    this->push(lhs->py__ne__(rhs));
                    break;
                case LESS_EQUAL:
                    this->push(lhs->py__le__(rhs));
                    break;
                case GREATER_EQUAL:
                    this->push(lhs->py__ge__(rhs));
                    break;
                case ByteCode::EXC_MATCH:
                {
                    // bool found = false;
                    // auto klass = lhs->as<TypeObject>()->get_own_klass();
                    // if (lhs == rhs)
                    // {
                    //     found = true;
                    // }
                    // else
                    // {
                    //     for (int i = 0; i < klass->get_mro()->size(); ++i)
                    //     {
                    //         if (lhs->get_klass()->get_mro()->get(i) == rhs)
                    //         {
                    //             found = true;
                    //             break;    
                    //         }
                    //     }
                    // }
                    bool found = lhs == rhs ? 
                        true :
                        lhs->get_klass()->contains_mro(rhs->get_klass());

                    this->push(found ? Universe::True : Universe::False);
                    break;
                }
                default: std::unreachable();
                }
                break;
            }
            case ByteCode::POP_JUMP_IF_TRUE:
            {
                auto v = pop();
                if (((Integer*)v)->value() == 1)
                    this->frame->pc = op_arg;
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
                if (w != Universe::None)
                {
                    this->push(w);
                    break;
                }

                w = this->frame->globals->get(v);
                if (w != Universe::None)
                {
                    this->push(w);
                    break;
                }

                w = this->buildin->get(v);
                if (w != Universe::None)
                {
                    this->push(w);
                    break;
                }

                this->push(Universe::None);
                break;
            }
            case ByteCode::RAISE_VARARGS:
            {
                PYTHON_ASSERT(1 <= op_arg && op_arg <= 3);
                Object* objs[3] = {};
                objs[op_arg - 1] = this->pop();
                this->do_raise(objs[0], objs[1], objs[2]);
                break;
            }
            case ByteCode::SETUP_EXCEPT:
            case ByteCode::SETUP_FINALLY:
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
                this->status = Status::IS_BREAK;
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
                if (w != Universe::None) {
                    this->push(w);
                    break;
                }

                w = this->buildin->get(v);
                if (w != Universe::None) {
                    this->push(w);
                    break;
                }

                this->push(Universe::None);
                break;
            }
            case ByteCode::BINARY_SUBSCR:
            {
                auto [rhs, lhs] = this->pop_top_two();
                this->push(lhs->py__getitem__(rhs));
                break;
            }
            case ByteCode::STORE_ATTR:
            {
                auto u = this->pop();
                auto v = this->frame->co_names->get(op_arg);
                auto w = this->pop();
                u->py__setattr__(v, w);
                break;
            }
            case ByteCode::LOAD_ATTR:
            {
                auto v = this->pop();
                auto w = this->frame->co_names->get(op_arg);
                auto attr = v->py__getattr__(w);
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
            case ByteCode::CONTINUE_LOOP:
            {
                this->status = Status::IS_CONTINUE;
                this->ret_value = (Object*)((long long) op_arg);
                break;
            }
            case ByteCode::END_FINALLY:
            {
                // Restore state before 'finally'
                auto v = this->pop();
                if (((long long) v) & 0x1)
                {
                    this->status = (Status)(((long long)v) >> 1);
                    if (this->status == Status::IS_RETURN)
                        this->ret_value = this->pop();
                    else if (this->status == Status::IS_CONTINUE)
                        this->frame->pc = (int)((long long)this->pop());
                }
                else if (v != Universe::None)
                {
                    this->exception_class = v;
                    this->pending_exception = this->pop();
                    this->trace_back = this->pop();
                    this->status = Status::IS_EXCEPTION;
                }
                break;
            }
            case ByteCode::STORE_SUBSCR:
            {
                auto key = this->pop();
                auto x = this->pop();
                auto value = this->pop();
                x->py__setitem__(key, value);
                break;
            }
            case ByteCode::UNPACK_SEQUENCE:
            {
                auto v = this->pop();
                while (op_arg--)
                {
                    this->push(v->py__getitem__(new Integer(op_arg)));
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
        
            // If current status is not IS_OK, try recover all operand 
            // of all blocks. If a block is SETUP_FINALLY, jump to final statement

            // Something like exception, continue or break happened
            while (this->status != Status::IS_OK && this->frame->loop_stack->size() != 0)
            {
                auto block = this->frame->loop_stack->get(this->frame->loop_stack->size() - 1);
                // Continue block
                if (this->status == Status::IS_CONTINUE && block->type == ByteCode::SETUP_LOOP)
                {
                    this->frame->pc = (int) ((long long) this->ret_value);
                    this->status = Status::IS_OK;
                    break;
                }

                block = this->frame->loop_stack->pop();
                while (this->stack_level() > block->level)
                {
                    this->pop();
                }

                if (this->status == Status::IS_BREAK && block->type == ByteCode::SETUP_LOOP)
                {
                    this->frame->pc = block->target;
                    this->status = Status::IS_OK;
                } 
                else if (block->type == ByteCode::SETUP_FINALLY || 
                         (this->status == Status::IS_EXCEPTION && block->type == ByteCode::SETUP_EXCEPT))
                {
                    if (this->status == Status::IS_EXCEPTION)
                    {
                        // Traceback , value and exception class
                        this->push(this->trace_back);
                        this->push(this->pending_exception);
                        this->push(this->exception_class);

                        this->trace_back = nullptr;
                        this->pending_exception = nullptr;
                        this->exception_class = nullptr;
                    }
                    else
                    {
                        if (this->status == Status::IS_RETURN || this->status == Status::IS_CONTINUE)
                        {
                            this->push(this->ret_value);
                        }
                        // Use the last bit to mark this is not a pointer
                        this->push((Object *) (((long) this->status << 1) | 0x1));
                    }

                    this->frame->pc = block->target;
                    this->status = Status::IS_OK;

                }
            }

            // Has pending exception and no handler found, unwind stack.
            // The status should be IS_OK before handling final block.
            if (this->status != Status::IS_OK && this->frame->loop_stack->size() == 0)
            {
                if (this->status == Status::IS_EXCEPTION)
                {
                    this->ret_value = nullptr;
                    this->trace_back->as<Traceback>()->record_frame(this->frame);
                }

                if (this->status == Status::IS_RETURN)
                {
                    this->status = Status::IS_OK;
                }

                // In such case, we don not need to remove current frame.
                if (this->frame->is_first_frame() || this->frame->is_entry_frame())
                {
                    return;
                }

                this->leave_frame();
            }
        }
    }

    void Interpreter::destroy_frame()
    {
        FrameObject* fo = this->frame;
        this->frame = this->frame->sender;
        delete fo;
    }

    void Interpreter::enter_frame(FrameObject* frame)
    {
        frame->sender = this->frame;
        this->frame = frame;
    }

    Interpreter::Status Interpreter::do_raise(Object* exception_type, Object* exception_instance, Object* traceback)
    {
        PYTHON_ASSERT(exception_type && "exception_type should not be nullptr?");
        this->status = Status::IS_EXCEPTION;
        if (!traceback)
        {
            traceback = new Traceback();
        }

        if (exception_instance)
        {
            this->exception_class = exception_type;
            this->pending_exception = exception_instance;
            this->trace_back = traceback;
            return Status::IS_EXCEPTION;
        }

        if (exception_type->get_klass() == TypeKlass::get_instance())
        {
            this->pending_exception = this->call_virtual(exception_type, nullptr);
            this->exception_class = exception_type;
        }
        else
        {
            this->pending_exception = exception_type;
            this->exception_class = this->pending_exception->get_klass()->get_type_object();
        }
        this->trace_back = traceback;
        return Status::IS_EXCEPTION;
    }

    void Interpreter::initialize()
    {
        // _builtins->extend(ModuleObject::import_module(new HiString("builtin")));
        // Universe::stop_iteration = _builtins->get(new HiString("StopIteration"));

        // _modules = new HiDict();
        // _modules->put(new HiString("__builtins__"), _builtins);
    }

    void Interpreter::leave_frame()
    {
        this->destroy_frame();
        this->push(this->ret_value);
    }

    Interpreter::Interpreter()
    {

        this->buildin = new Dict();

        /* buildin variables */
        this->buildin->put(new String("True"), Universe::True);
        this->buildin->put(new String("False"), Universe::False);
        this->buildin->put(new String("None"), Universe::None);

        /* buildin functions */
        this->buildin->put(new String("len"), new FunctionObject(native::len));
        this->buildin->put(new String("isinstance"), new FunctionObject(native::isinstance));
        this->buildin->put(new String("type"), new FunctionObject(native::type));
        this->buildin->put(new String("hash"), new FunctionObject(native::hash));
        this->buildin->put(new String("iter"), new FunctionObject(native::iter));
        this->buildin->put(new String("next"), new FunctionObject(native::next));
        this->buildin->put(new String("map"), new FunctionObject(native::map));
        this->buildin->put(new String("filter"), new FunctionObject(native::filter));
        this->buildin->put(new String("take"), new FunctionObject(native::take));
        this->buildin->put(new String("drop"), new FunctionObject(native::drop));

        /* buildin types */
        this->buildin->put(new String("int"), IntegerKlass::get_instance()->get_type_object());
        this->buildin->put(new String("object"), ObjectKlass::get_instance()->get_type_object());
        this->buildin->put(new String("str"), StringKlass::get_instance()->get_type_object());
        this->buildin->put(new String("list"), ListKlass::get_instance()->get_type_object());
        this->buildin->put(new String("dict"), DictKlass::get_instance()->get_type_object());

        this->buildin->put(new String("BaseException"), BaseExceptionKlass::get_instance()->get_type_object());
        this->buildin->put(new String("Exception"), ExceptionKlass::get_instance()->get_type_object());
        this->buildin->put(new String("StopIteration"), StopIterationKlass::get_instance()->get_type_object());

        // Not necessary
        // this->buildin->put(new String("list_iterator"), ListIteratorKlass::get_instance()->get_type_object());


        // TESTING:
    }

    Object* Interpreter::call_virtual(Object* func, List* args)
    {
        if (func->get_klass() == NativeFunctionKlass::get_instance()) {
            // we do not create a virtual frame, but native frame.
            return func->as<FunctionObject>()->call(args);
        }
        else if (func->get_klass() == MemberFunctionKlass::get_instance()) {
            auto method = func->as<MemberFunctionObject>();
            if (!args) {
                args = new List(1);
            }
            args->insert(0, method->owner);
            return this->call_virtual(method->func, args);
        }
        else if (MemberFunctionObject::is_function(func)) {
            int size = args ? args->size() : 0;
            FrameObject* frame = new FrameObject((FunctionObject*) func, args, size);
            // frame->set_entry_frame(true);
            frame->entry_frame = true;
            this->enter_frame(frame);
            this->eval_frame();
            this->destroy_frame();
            return this->ret_value;
        }

        return Universe::None;
    }

    Object* Interpreter::eval_generator(Generator* g)
    {
        // Eval frame of Generator
        this->enter_frame(g->frame);
        g->frame->entry_frame = true;
        this->eval_frame();

        if (this->status != Status::IS_YIELD)
        {
            this->status = Status::IS_OK;
            this->leave_frame();
            g->frame = nullptr;
            return nullptr;
        }

        this->status = Status::IS_OK;
        this->frame = this->frame->sender;
        
        return this->ret_value;
    }

    void Interpreter::run(CodeObject* codes)
    {
        this->frame = new FrameObject(codes);
        this->frame->locals->put(StringTable::name, new String("__main__"));
        this->eval_frame();

        if (this->status == Status::IS_EXCEPTION)
        {
            this->status = Status::IS_OK;
            this->trace_back->print();
            this->pending_exception->print();
            std::cout << '\n';

            this->trace_back = nullptr;
            this->pending_exception = nullptr;
            this->exception_class = nullptr;
        }

        this->destroy_frame();

    }

    Interpreter::Status Interpreter::do_raise_stop_iteration_exception(std::string message)
    {
        auto args = new List();
        args->append(new String(std::move(message)));
        auto stop_iteration = StopIterationKlass::get_instance()->get_type_object();
        auto instance = this->call_virtual(stop_iteration, args);
        return this->do_raise(
            stop_iteration,
            instance, 
            nullptr
        );
    }

}