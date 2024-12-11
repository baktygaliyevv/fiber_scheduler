#pragma once
#include "fiber.hpp"
#include "../context/context.hpp"
#include <deque>

using namespace std;

class Scheduler {
private:
    std::deque<Fiber*> fibers_;

public:
    Fiber* current_fiber_ = nullptr;

    void spawn(Fiber* fiber) {
        fibers_.push_back(fiber);
    }

    void do_it() {
        while (!fibers_.empty()) {
            current_fiber_ = fibers_.front();
            fibers_.pop_front();

            // Switch to the fiber's context
            swap_context(&main_context, current_fiber_->get_context());

            if (current_fiber_) {
                fibers_.push_back(current_fiber_);
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
            fibers_.push_back(current_fiber_);
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