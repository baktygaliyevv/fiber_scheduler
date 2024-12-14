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

cpp
```
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
- Utilizes get_context and set_context from the context library.
- Ensures stack alignment according to the SysV ABI.

**Outcome:** Demonstrates the ability to switch context within the same thread. This serves as the foundation for creating fibers with independent stacks.

---

### Task 2: Fiber and Scheduler Classes
**Objective**: Develop a `Fiber` class to represent a lightweight task and a `Scheduler` to manage and execute these tasks.

**Fiber Class**:
cpp
```
https://github.com/baktygaliyevv/fiber_scheduler/blob/b3008fcc91c931c5714afd042540134834de7a41/include/fiber.hpp
```

**Scheduler Class**:
cpp 
```
https://github.com/baktygaliyevv/fiber_scheduler/blob/b3008fcc91c931c5714afd042540134834de7a41/include/scheduler.hpp
```
**Key Features**:
- Implements a round-robin scheduling algorithm.
- Supports task priority for execution order.
- Allows fibers to yield control back to the scheduler.

**Test Example**:
cpp
```
https://github.com/baktygaliyevv/fiber_scheduler/blob/b3008fcc91c931c5714afd042540134834de7a41/src/main.cpp
```

---

### Task 3: Yield and Data Sharing
**Objective:** Extend the scheduler to support fiber yielding and data sharing.

**Implementation:**
- **Yield:** Allows fibers to pause execution and rejoin the scheduler’s queue.
- **Data Sharing:** Pass data to fibers at creation and enable them to modify shared data.

**Example:**
cpp
```

```

## Additional Features
### Task Priorities: Fibers with higher priorities execute first.

### Debugging Mode: Logs the state of the scheduler and tasks.

## Testing
The project includes a comprehensive testing suite using the `SimpleTest` framework.

**Test Coverage:**
- 
- 
- 
- 
- 
- 
- 

## Build and Run
Talk about .sh files to build and run by one command

## Screenshots

## Conclusion
This project successfully implements a cooperative fiber scheduler in C++, demonstrating key concepts of task scheduling, context switching, and priority management. The inclusion of additional features like debugging and priority-based execution enhances its utility and performance.

