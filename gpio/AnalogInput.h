/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_GPIO_ANALOGINPUT_H
#define _ESP_GPIO_ANALOGINPUT_H

#include "library.h"
#include "Pin.h"

F_BEGIN_NAMESPACE

class AnalogInput : public Pin
{
public:
    AnalogInput(gpio_num_t pin);
    virtual ~AnalogInput() {}

    void setPullMode(gpio_pull_mode_t mode);    
};

F_END_NAMESPACE

#endif // _ESP_GPIO_ANALOGINPUT_H