#pragma once
#include "../context/context.hpp"

class Fiber {
    private:
        Context context_;
        void* data_;
    public:
        Fiber(void (*function)(), void* data = nullptr) {
            char* stack = new char[4096];
            void* sp = stack + 4096;

            uintptr_t sp_val = reinterpret_cast<uintptr_t>(sp);
            sp_val &= ~((uintptr_t)0xF);
            sp = reinterpret_cast<void*>(sp_val);

            sp = static_cast<char*>(sp) - 128;

            context_.rip = reinterpret_cast<void*>(function);
            context_.rsp = sp;

            context_.rbx = nullptr;
            context_.rbp = nullptr;
            context_.r12 = nullptr;
            context_.r13 = nullptr;
            context_.r14 = nullptr;
            context_.r15 = nullptr;
        }
        Context* get_context() {
            return &context_;
        }
        void* get_data() {
            return data_;
        }
    };
