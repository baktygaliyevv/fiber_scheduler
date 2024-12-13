#pragma once
#include "fiber.hpp"
#include "../context/context.hpp"
#include <queue>
#include <vector>
#include <functional>

// 1, 4, 7, 8 optional, 15
using namespace std;

struct FiberComparator {
    bool operator()(const Fiber* a, const Fiber* b) {
        return a->get_priority() < b->get_priority();
    }
};

class Scheduler {
private:
    priority_queue<Fiber*, vector<Fiber*>, FiberComparator> fibers_;

public:
    Fiber* current_fiber_ = nullptr;

    void spawn(Fiber* fiber) {
        fibers_.push(fiber);
    }

    void do_it() {
        while (!fibers_.empty()) {
            current_fiber_ = fibers_.top();
            fibers_.pop();

            // Switch to the fiber's context
            swap_context(&main_context, current_fiber_->get_context());

            if (current_fiber_) {
                fibers_.push(current_fiber_);
            }
        }
    }

    void fiber_exit() {
        Fiber* exiting_fiber = current_fiber_;
        current_fiber_ = nullptr;
        swap_context(exiting_fiber->get_context(), &main_context);
    }

    void yield() {
        if (current_fiber_) {
            fibers_.push(current_fiber_);
            Fiber* temp = current_fiber_;
            current_fiber_ = nullptr;
            swap_context(temp->get_context(), &main_context);
            current_fiber_ = temp;
        }
        // } else {
        //     swap_context(&main_context, &main_context);
        // }
    }

    Context main_context;
};