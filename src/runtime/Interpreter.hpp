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

        FrameObject* m_frame = nullptr;

        Object* m_ret_value = nullptr;  // Save return value for RETURN_VALUE and start position for CONTINUE

        Dict* m_buildin = nullptr;

        Status m_status = Status::IS_OK;

        Object* m_exception_class = nullptr;  // Record the exception type such as `StopIteration`
        
        Object* m_pending_exception = nullptr;  // Record the exception instance such as `StopIteration()`

        Object* m_trace_back = nullptr;  

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

    public:

        Interpreter();

        void initialize();

        Status do_raise(Object* exception_type, Object* exception_instance, Object* traceback);

        Object* call_virtual(Object* func, List* args);

        Object* eval_generator(class Generator* g);

        void run(CodeObject* codes);

        Status do_raise_stop_iteration_exception(std::string message);
    };
}



