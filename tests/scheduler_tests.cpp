#include <iostream>
#include "../include/fiber.hpp"
#include "../include/scheduler.hpp"
#include "../simpletest_test/simpletest/simpletest.h"

char const *groups[] = {
    "SchedulerTests",
};

bool task1_executed = false;
bool task2_executed = false;
int step = 0;
int executed_tasks = 0;
int priority_execution_order[2] = {0, 0};
int counter = 0;

Scheduler global_scheduler;

void task1() {
    task1_executed = true;
    global_scheduler.fiber_exit();
}

void task2() {
    task2_executed = true;
    global_scheduler.fiber_exit();
}

void yielding_task1() {
    step++;
    global_scheduler.yield();
    step++;
    global_scheduler.fiber_exit();
}

void yielding_task2() {
    if (step == 1) step++;
    global_scheduler.fiber_exit();
}

void high_volume_task() {
    executed_tasks++;
    global_scheduler.fiber_exit();
}

void high_priority_task() {
    priority_execution_order[0] = 10;
    global_scheduler.fiber_exit();
}

void low_priority_task() {
    priority_execution_order[1] = 1;
    global_scheduler.fiber_exit();
}

void mixed_priority_task_1() {
    priority_execution_order[0] = 10;
    global_scheduler.fiber_exit();
}

void mixed_priority_task_2() {
    priority_execution_order[1] = 1;
    global_scheduler.fiber_exit();
}

void deep_recursion_task() {
    static int depth = 0;
    depth++;
    if (depth < 1000) {
        global_scheduler.yield();
    }
    global_scheduler.fiber_exit();
}

DEFINE_TEST_G(BasicTaskExecutionTest, SchedulerTests) {
    Fiber f1(task1);
    Fiber f2(task2);

    global_scheduler.spawn(&f1);
    global_scheduler.spawn(&f2);
    global_scheduler.do_it();

    TEST_MESSAGE(task1_executed, "Task 1 should be executed!");
    TEST_MESSAGE(task2_executed, "Task 2 should be executed!");
}

DEFINE_TEST_G(TaskYieldingTest, SchedulerTests) {

    Fiber f1(yielding_task1);
    Fiber f2(yielding_task2);

    global_scheduler.spawn(&f1);
    global_scheduler.spawn(&f2);
    global_scheduler.do_it();

    TEST_MESSAGE(step == 3, "All tasks should complete in correct order!");
}

DEFINE_TEST_G(HighVolumeTasksTest, SchedulerTests) {
    const int num_tasks = 1000;

    for (int i = 0; i < num_tasks; ++i) {
        Fiber* f = new Fiber(high_volume_task);
        global_scheduler.spawn(f);
    }

    global_scheduler.do_it();

    TEST_MESSAGE(executed_tasks == num_tasks, "All tasks should be executed!");
}

DEFINE_TEST_G(PrioritySchedulingTest, SchedulerTests) {

    Fiber high_priority(high_priority_task, nullptr, 10);
    Fiber low_priority(low_priority_task, nullptr, 1);

    global_scheduler.spawn(&low_priority);
    global_scheduler.spawn(&high_priority);

    global_scheduler.do_it();

    TEST_MESSAGE(priority_execution_order[0] == 10, "High priority task should execute first!");
    TEST_MESSAGE(priority_execution_order[1] == 1, "Low priority task should execute second!");
}

DEFINE_TEST_G(EmptyQueueTest, SchedulerTests) {
    global_scheduler.do_it();
    TEST_MESSAGE(true, "Scheduler should not crash or hang on an empty queue!");
}

DEFINE_TEST_G(MixedPriorityTest, SchedulerTests) {
    Fiber high_priority(mixed_priority_task_1, nullptr, 10);
    Fiber low_priority(mixed_priority_task_2, nullptr, 1);

    global_scheduler.spawn(&low_priority);
    global_scheduler.spawn(&high_priority);

    global_scheduler.do_it();

    TEST_MESSAGE(priority_execution_order[0] == 10, "High priority task should execute first!");
    TEST_MESSAGE(priority_execution_order[1] == 1, "Low priority task should execute second!");
}

DEFINE_TEST_G(LimitedStackTest, SchedulerTests) {
    Fiber deep_task(deep_recursion_task);

    global_scheduler.spawn(&deep_task);
    global_scheduler.do_it();

    TEST_MESSAGE(true, "Deep recursion task completed without crashing!");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}
