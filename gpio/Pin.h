/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_GPIO_PIN_H
#define _ESP_GPIO_PIN_H

#include "library.h"
#include "driver/gpio.h"

F_BEGIN_NAMESPACE

class Pin
{
public:
    Pin(gpio_num_t pin, gpio_mode_t direction);

    void setInverted(bool state);

    gpio_num_t pin() const { return _pin; }
    bool isInverted() const { return _isInverted; }

private:
    gpio_num_t _pin;
    bool _isInverted;
};

F_END_NAMESPACE

#endif // _ESP_GPIO_PIN_H