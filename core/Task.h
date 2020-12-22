/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_CORE_TASK_H
#define _ESP_NATIVE_CORE_TASK_H

#include "../library.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

F_BEGIN_NAMESPACE

class Task
{
    F_DISABLE_COPY(Task);

public:
    enum core_t : int {
        CORE_ANY = -1,
        CORE_0 = 0,
        CORE_1 = 1
    };

    /// Suspends all but the calling task.
    static void suspendAll();
    /// Resumes task scheduling after a call to suspendAll().
    static void resumeAll();
    /// Delays the calling task by the given number of milliseconds.
    static void delay(uint32_t milliseconds);
    /// Returns the number of tasks the kernel is currently managing.
    static uint32_t count();
    
    /// Creates and runs a new task.
    Task(const char* pName = "", core_t core = CORE_ANY, uint32_t stackSize = 8192);
    /// Virtual destructor. Stops and deletes the task.
    virtual ~Task();

    /// Entry method of the task. Called when the task starts. Must never return.
    virtual void run() = 0;

    /// Delay task until given time, measured from task start time.
    void delayUntil(uint32_t millisecondsTime);
    /// Suspends the task.
    void suspend() { vTaskSuspend(_taskHandle); }
    /// Resumes the task.
    void resume() { vTaskResume(_taskHandle); }
    /// Sets the priority of this task.
    void setPriority(uint32_t priority) { vTaskPrioritySet(_taskHandle, priority); }

    /// Returns the time when the task has started in milliseconds.
    uint32_t startTime() const { return _taskStartTicks * portTICK_PERIOD_MS; }
    /// Returns the task's priority.
    uint32_t priority() const { return uxTaskPriorityGet(_taskHandle); };
    /// Returns the task's state.
    eTaskState state() const { return eTaskGetState(_taskHandle); }
    /// Returns the task's free stack space in bytes.
    uint32_t stackHighWaterMark() const { return uxTaskGetStackHighWaterMark(_taskHandle) * 4; }

    /// Returns the task's name.
    const char* name() const { return pcTaskGetTaskName(_taskHandle); }
    /// Returns the task's internal handle.
    TaskHandle_t handle() const { return _taskHandle; }

private:
    static void _taskEntry(void* pParam);

    TaskHandle_t _taskHandle;
    uint32_t _taskStartTicks;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_CORE_TASK_H