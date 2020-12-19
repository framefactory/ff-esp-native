/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_DIGITALINPUT_H
#define _ESP_NATIVE_DIGITALINPUT_H

#include "../library.h"
#include "Pin.h"

#include "esp_timer.h"


F_BEGIN_NAMESPACE

class DigitalInput : public Pin
{
public:
    DigitalInput(int pin);
    virtual ~DigitalInput();

    void setPull(gpio_pull_mode_t pull);
    void enableInterrupt(gpio_int_type_t type, uint64_t usecsDebounce = 0);

    bool level() const;
    bool isHigh() const { return level(); }
    bool isLow() const { return !level(); }

protected:
    void onChange(bool level);

private:
    static void _interruptHandler(void* arg);
    static void _timerHandler(void* arg);

    mutable bool _level;
    gpio_int_type_t _interruptType;
    uint64_t _debounceDelay;
    esp_timer_handle_t _debounceTimer;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_DIGITALINPUT_H