#include <iostream>
#include "../include/fiber.hpp"
#include "../context/context.hpp"

Context scheduler_context;

volatile int state = 0;

extern "C" void fiber_exit() {
    set_context(&scheduler_context);
}

void foo() {
    std::cout << "foo" << std::endl;
    fiber_exit();
}

void bar() {
    std::cout << "bar" << std::endl;
    fiber_exit();
}

int main() {
    Fiber fiberFoo(foo);
    Fiber fiberBar(bar);

    Context* contextFoo = fiberFoo.get_context();
    Context* contextBar = fiberBar.get_context();

    std::cout << "switching to foo" << std::endl;
    get_context(&scheduler_context);
    if (state == 0) {
        state = 1;
        set_context(contextFoo); 
    }

    std::cout << "switching to bar" << std::endl;
    get_context(&scheduler_context);
    if (state == 1) {
        state = 2;
        set_context(contextBar);
    }
    
    std::cout << "back to main" << std::endl;

    return 0;
}
