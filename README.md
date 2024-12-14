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

---

## Tasks