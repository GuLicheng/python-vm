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

        FrameObject* frame = nullptr;

        Object* ret_value = nullptr;  // Save return value for RETURN_VALUE and start position for CONTINUE

        Dict* buildin = nullptr;

        Status status = Status::IS_OK;

        Object* exception_class = nullptr;  // Record the exception type such as `StopIteration`
        
        Object* pending_exception = nullptr;  // Record the exception instance such as `StopIteration()`

        Object* trace_back = nullptr;  

    private:
    
        void push(Object* x);

        Object* pop();

        Object* top();

        std::pair<Object*, Object*> pop_top_two();

        int stack_level() const;

        void build_frame(Object* callable, List* args, int op_arg);

        void eval_frame();

        void leave_frame();

        void destroy_frame();

        void enter_frame(FrameObject* frame);

        Status do_raise(Object* exception_type, Object* exception_instance, Object* traceback);

    public:

        Interpreter();

        void initialize();

        Object* call_virtual(Object* func, List* args);

        Object* eval_generator(class Generator* g);

        void run(CodeObject* codes);

    };
}



