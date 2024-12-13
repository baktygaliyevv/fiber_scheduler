#pragma once
#include "fiber.hpp"
#include "../context/context.hpp"
#include <queue>
#include <vector>
#include <functional>
#include <iostream>

// 7, 8 optional, 15
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

    void do_it(bool debug_mode) {
        while (!fibers_.empty()) {
            current_fiber_ = fibers_.top();
            fibers_.pop();
            if (debug_mode) debug();

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

    void debug() {
        cout << "\nScheduler State" << endl;
        cout << "Current fiber: ";
        if (current_fiber_) {
            cout << "priority: " << current_fiber_->get_priority() << endl;
        } else {
            cout << "none" << endl;
        }

        cout << "Tasks in queue: " << fibers_.size() << endl;
        if (!fibers_.empty()) {
            cout << "Queue:" << endl;

            vector<Fiber*> temp_queue;
            temp_queue.reserve(fibers_.size());
            while (!fibers_.empty()) {
                temp_queue.push_back(fibers_.top());
                fibers_.pop();
            }

            for (auto f : temp_queue) {
                cout << "  - Fiber priority: " << f->get_priority() << endl;
                fibers_.push(f);
            }
        } else {
            cout << "Queue is empty" << endl;
        }
    }


    Context main_context;
};