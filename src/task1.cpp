#include "../include/fiber.hpp"
#include <cstring>
#include <stdexcept>
#include <cstdint>

Fiber::Fiber(void (*function)(), void* data) : data_(data) {
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

Context* Fiber::get_context() {
    return &context_;
}

void* Fiber::get_data() {
    return data_;
}
