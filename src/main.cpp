#include <iostream>
#include "../include/fiber.hpp"
#include "../context/context.hpp"

void foo() {
    std::cout << "foo" << std::endl;
}

void bar() {
    std::cout << "bar" << std::endl;
}

int main() {
    Fiber fiberFoo(foo);
    Fiber fiberBar(bar);

    Context* contextFoo = fiberFoo.get_context();
    Context* contextBar = fiberBar.get_context();

    std::cout << "switching to foo" << std::endl;
    set_context(contextFoo);

    std::cout << "switching to bar" << std::endl;
    set_context(contextBar);

    std::cout << "back to main" << std::endl;

    return 0;
}
