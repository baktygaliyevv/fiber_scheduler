#include "../include/fiber.hpp"
#include <cstring> 
#include <stdexcept>

Fiber::Fiber(void (*function)(), void* data) : data_(data) {
    char* stack = new char[4096];

    void* sp = stack + 4096;                     
    sp = reinterpret_cast<void*>((uintptr_t)sp & ~0xF);
    sp = static_cast<char*>(sp) - 128;

    context_.rip = reinterpret_cast<void*>(function);
    context_.rsp = sp;
}

Context* Fiber::get_context() {
    return &context_;
}

void* Fiber::get_data() {
    return data_;
}
