#pragma once
#include "../context/context.hpp"

class Fiber {
    private:
        Context context_;
        void* data_;
    public:
        Fiber(void (*function)(), void* data = nullptr);
        Context* get_context();
        void* get_data();
    };
