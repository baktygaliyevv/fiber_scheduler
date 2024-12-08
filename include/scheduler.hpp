#pragma once
#include "fiber.hpp"
#include "context/context.hpp"
#include <deque>

using namespace std;

class Scheduler {
private:
    std::deque<Fiber*> fibers_;
    Context* main_context_;
public:
    void spawn(Fiber* fiber);
    void do_it();
    void fiber_exit();
    void yield();
};
