#pragma once
#include "../context/context.hpp"

class Fiber {
    private:
        Context context_;
        void* data_;
        void* stack_;
        int priority_;
    public:
        Fiber(void (*function)(), void* data = nullptr, int priority = 0): priority_(priority) {
            data_ = data;

            char* stack = new char[4096];
            void* sp = stack + 4096;

            uintptr_t sp_val = reinterpret_cast<uintptr_t>(sp);
            sp_val &= ~((uintptr_t)0xF);
            sp = reinterpret_cast<void*>(sp_val);

            sp = static_cast<char*>(sp) - 128;

            context_.rip = reinterpret_cast<void*>(function);
            context_.rsp = sp;
        }
        Context* get_context() {
            return &context_;
        }
        void* get_data() {
            return data_;
        }
        int get_priority() const {
        return priority_;
    }
    };
