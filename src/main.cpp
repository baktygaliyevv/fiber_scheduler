#include <iostream>
#include "../include/fiber.hpp"
#include "../include/scheduler.hpp"
#include "../context/context.hpp"

using namespace std;

Scheduler s;

void func1() {
    cout << "fiber 1" << endl;
    int* dp = static_cast<int*>(s.current_fiber_->get_data());
    cout << "fiber 1: " << *dp << endl;
    *dp += 1;
    s.fiber_exit();
}

void func2() {
    int* dp = static_cast<int*>(s.current_fiber_->get_data());
    cout << "fiber 2: " << *dp << endl;
    s.fiber_exit();
}

int main() {
    int d = 10;
    int* dp = &d;
    Fiber f2(func2, dp);
    Fiber f1(func1, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();
    return 0;
}
