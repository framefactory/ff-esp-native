/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_PIN_H
#define _ESP_NATIVE_PIN_H

#include "../library.h"
#include "driver/gpio.h"

F_BEGIN_NAMESPACE

class Pin
{
    F_DISABLE_COPY(Pin);

public:
    Pin(int pin = 0, gpio_mode_t direction = GPIO_MODE_OUTPUT);
    virtual ~Pin();

    void setInverted(bool state);

    gpio_num_t pin() const { return _pin; }
    bool isInverted() const { return _isInverted; }
    bool isValid() const { return _isValid; }

private:
    gpio_num_t _pin = GPIO_NUM_0;
    bool _isInverted = false;
    bool _isValid = false;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_PIN_H