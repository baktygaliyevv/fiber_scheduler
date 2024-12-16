# Fiber Scheduler (C++)

## Introduction
This project implements a Fiber Scheduler in C++ as part of the Advanced Systems Programming module. The primary goal is to provide a simple runtime that supports cooperative multitasking using fibers, which are lightweight user-space threads. This implementation focuses on cooperative scheduling, where control is explicitly yielded back to the scheduler by the fiber. The scheduler uses priority-based task execution and includes additional features for debugging and testing.

## Project Overview
The project is structured as follows:
- **Context Submodule:** Provides low-level assembly code for saving and restoring execution context
- **SimpleTest Submodule:** A lightweight testing framework for validating the fiber scheduler
- **Fiber and Scheduler Implementation:** Encapsulates fiber creation, scheduling, and task execution
- **Testing Suite:** Unit tests to verify the functionality of the scheduler and fibers

The repository includes the following submodules:
- `context`: [Context Repository](https://gitlab.uwe.ac.uk/br-gaster/context)
- `simpletest`: [Simpletest Repository](https://github.com/kudaba/simpletest_test)

## Tasks

### Task 1: Basic Context Switching
**Objective:** Implement low-level context switching using the context library. This involves saving and restoring the CPU state, including registers and stack pointers.

**Implementation:**
```cpp
// code from our repo should be added, this is a temporary code as an example
volatile int x = 0;
Context c;

if (get_context(&c) == 0) {
    x++;
    set_context(&c);
}
std::cout << "x: " << x << std::endl;
```
**Features:**
- Utilizes `get_context` and `set_context` from the context library.
- Ensures stack alignment according to the SysV ABI.

**Outcome:** Demonstrates the ability to switch context within the same thread. This serves as the foundation for creating fibers with independent stacks.

---

### Task 2: Fiber and Scheduler Classes
**Objective**: Develop a `Fiber` class to represent a lightweight task and a `Scheduler` to manage and execute these tasks.

**Fiber Class**:
```hpp
class Fiber {
    private:
        Context context_;
        void* data_;
        void* stack_;
    public:
        Fiber(void (*function)(), void* data = nullptr);
        Context* get_context();
        void* get_data();
};
```

**Scheduler Class**:
```hpp
class Scheduler {
private:
    std::deque<Fiber*> fibers_;

public:
    Fiber* current_fiber_ = nullptr;
    void spawn(Fiber* fiber);
    void do_it();
    void fiber_exit();
    void yield();
    Context main_context;
};
```

**Key Features**:
- Implements a round-robin scheduling algorithm.
- Supports task priority for execution order.
- Allows fibers to yield control back to the scheduler.

**Test Example**:
```cpp
DEFINE_TEST_G(BasicTaskExecutionTest, SchedulerTests) {
    Fiber f1(task1);
    Fiber f2(task2);

    global_scheduler.spawn(&f1);
    global_scheduler.spawn(&f2);
    global_scheduler.do_it();

    TEST_MESSAGE(task1_executed, "Task 1 should be executed!");
    TEST_MESSAGE(task2_executed, "Task 2 should be executed!");
}

```

---

### Task 3: Yield and Data Sharing
**Objective:** Extend the scheduler to support fiber yielding and data sharing.

**Implementation:**
- **Yield:** Allows fibers to pause execution and rejoin the scheduler’s queue.
- **Data Sharing:** Pass data to fibers at creation and enable them to modify shared data.

**Example:**
```cpp
void yielding_task1() {
    step++;
    global_scheduler.yield();
    step++;
    global_scheduler.fiber_exit();
}
DEFINE_TEST_G(TaskYieldingTest, SchedulerTests) {

    Fiber f1(yielding_task1);
    Fiber f2(yielding_task2);

    global_scheduler.spawn(&f1);
    global_scheduler.spawn(&f2);
    global_scheduler.do_it();

    TEST_MESSAGE(step == 3, "All tasks should complete in correct order!");
}
```

## Additional Features
### Task Priorities: Fibers with higher priorities execute first.
To enhance the scheduler, priority-based execution was introduced using a `std::priority_queue`. This allows fibers with higher priorities to execute before those with lower priorities. If no priority is explicitly set during fiber creation, the default priority value of 0 is used, meaning such fibers will be treated as low-priority tasks in comparison to higher priority values.
**Implementation Changes:**
- Replaced the original `std::deque` with `std::priority_queue` in the Scheduler class.
- Added a `FiberComparator` struct to compare fibers based on their priority:
```cpp
struct FiberComparator {
    bool operator()(const Fiber* a, const Fiber* b) {
        return a->get_priority() < b->get_priority();
    }
};
```
- Updated the `Scheduler`’s `spawn()` and `do_it()` methods to use the priority queue:
```cpp
class Scheduler {
private:
    std::priority_queue<Fiber*, std::vector<Fiber*>, FiberComparator> fibers_;
    Context main_context_;

public:
    void spawn(Fiber* fiber) {
        fibers_.push(fiber);
    }

    void do_it() {
        while (!fibers_.empty()) {
            current_fiber_ = fibers_.top();
            fibers_.pop();

            swap_context(&main_context_, current_fiber_->get_context());

            if (current_fiber_) {
                fibers_.push(current_fiber_);
            }
        }
    }
};
```
**Example Usage**:
```cpp
Fiber f1(func1, nullptr, 1);  // Low priority
Fiber f2(func2, nullptr, 10); // High priority
Fiber f3(func3); // Default priority (0)
scheduler.spawn(&f1);
scheduler.spawn(&f2);
scheduler.do_it();
```
Output:
```
fiber 2
fiber 1
fiber 3
```

### Debugging Mode: Logs the state of the scheduler and tasks.
A debug mode was added to the scheduler to provide insights into its internal state during execution. By default, debug mode is disabled (`debug_mode = false`), but it can be enabled by setting it to `true` when calling the `do_it` function.

**Implementation:**
- A `debug` function was added to the `Scheduler` class to print the current state of the scheduler, including:
    - The currently executing fiber and its priority.
    - The number of tasks in the queue.
    - A detailed list of fibers in the queue with their priorities.
- Updated do_it to accept an optional debug_mode parameter:
```cpp
void do_it(bool debug_mode = false) {
    while (!fibers_.empty()) {
        current_fiber_ = fibers_.top();
        fibers_.pop();
        if (debug_mode) debug();

        swap_context(&main_context, current_fiber_->get_context());

        if (current_fiber_) {
            fibers_.push(current_fiber_);
        }
    }
}
```
**Debug Function:**
```cpp
void debug() {
    std::cout << "\nScheduler State" << std::endl;
    std::cout << "Current fiber: ";
    if (current_fiber_) {
        std::cout << "priority: " << current_fiber_->get_priority() << std::endl;
    } else {
        std::cout << "none" << std::endl;
    }

    std::cout << "Tasks in queue: " << fibers_.size() << std::endl;
    if (!fibers_.empty()) {
        std::cout << "Queue:" << std::endl;
        std::vector<Fiber*> temp_queue;
        while (!fibers_.empty()) {
            temp_queue.push_back(fibers_.top());
            fibers_.pop();
        }
        for (auto f : temp_queue) {
            std::cout << "  - Fiber priority: " << f->get_priority() << std::endl;
            fibers_.push(f);
        }
    } else {
        std::cout << "Queue is empty" << std::endl;
    }
}
```
**Example Usage:**
```cpp
bool debug_mode = true;
Fiber f1(func1, nullptr, 1);
Fiber f2(func2, nullptr, 2);
scheduler.spawn(&f1);
scheduler.spawn(&f2);
scheduler.do_it(debug_mode);
```
Output:
```
Scheduler State
Current fiber: priority: 2
Tasks in queue: 1
Queue:
  - Fiber priority: 1
Scheduler State
Current fiber: priority: 1
Tasks in queue: 0
Queue is empty
```
## Testing
The project includes a comprehensive testing suite using the `SimpleTest` framework.

**Test Coverage:**
- **Basic Task Execution Test:** Verifies that tasks are executed correctly in the scheduler.
- **Task Yielding Test:** Ensures tasks yield and resume execution in the correct order.
- **High Volume Tasks Test:** Validates the scheduler’s ability to handle a large number of tasks.
- **Priority Scheduling Test:** Confirms tasks with higher priority execute first.
- **Empty Queue Test:** Ensures the scheduler handles an empty queue gracefully.
- **Limited Stack Test:** Tests deep recursion and limited stack scenarios.


## Build and Run
**Build:**
To compile and run the project, two utility scripts are provided:

`run.sh`
This script builds and executes the main program.
```bash
#!/bin/bash
g++ -std=c++17 -o main.o main.cpp context/context.s

if [ $? -eq 0 ]; then
    chmod +x main.o
    ./main.o
else
    echo "Build failed"
fi
```
- **Functionality:** Compiles the main program and runs it if the build succeeds.
- **Usage:** Simply execute `./run.sh` from the terminal.

`run_tests.sh`
This script builds and executes the unit tests.
```bash
#!/bin/bash
g++ -std=c++17 -o scheduler_tests.o \
    tests/scheduler_tests.cpp simpletest_test/simpletest/simpletest.cpp context/context.s \
    -Iinclude -Isimpletest_test/simpletest \
    -lstdc++

if [ $? -eq 0 ]; then
    ./scheduler_tests.o
else
    echo "Build failed"
fi
```
- **Functionality:** Compiles the test suite and runs it if the build succeeds.
- **Usage:** Execute `./run_tests.sh` to validate the implementation.

## Screenshots
**run.sh stdout**

![screen 1](https://github.com/baktygaliyevv/fiber_scheduler/blob/8267d9462d8b893fef2153ad37bf41d2c73a5892/assets/Code_BuWkC0MxdS.png)

**run_tests.sh stdout**

![screen 2](https://github.com/baktygaliyevv/fiber_scheduler/blob/8267d9462d8b893fef2153ad37bf41d2c73a5892/assets/Code_vtOq7Bcglu.png)

## Conclusion
This project successfully implements a cooperative fiber scheduler in C++, demonstrating key concepts of task scheduling, context switching, and priority management. The inclusion of additional features like debugging and priority-based execution enhances its utility and performance.

