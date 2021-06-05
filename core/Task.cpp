/**
 * ESP/Native Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Task.h"
#include "esp_log.h"

F_USE_NAMESPACE

void Task::suspendAll()
{
    vTaskSuspendAll();
}

void Task::resumeAll()
{
    xTaskResumeAll();
}

void Task::delay(uint32_t milliseconds)
{
    vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}

uint32_t Task::count()
{
    return uxTaskGetNumberOfTasks();
}

Task::Task(const char* pName, core_t core, uint32_t priority, uint32_t stackSize)
{
    BaseType_t affinity = core < 0 || core > 1 ? tskNO_AFFINITY : core;

    BaseType_t result = xTaskCreatePinnedToCore(
        &Task::_taskEntry,
        pName,
        stackSize / 4,
        this,
        priority,
        &_taskHandle,
        affinity
    );

    if (result != pdPASS) {
        // handle error
    }
}

Task::~Task()
{
    vTaskDelete(_taskHandle);
    _taskHandle = nullptr;
}

void Task::delayUntil(uint32_t time)
{
    vTaskDelayUntil(&_taskStartTicks, time / portTICK_PERIOD_MS);
}

void Task::_taskEntry(void* pParam)
{
    Task* pTask = static_cast<Task*>(pParam);
    pTask->_taskStartTicks = xTaskGetTickCount();
    pTask->run();
}