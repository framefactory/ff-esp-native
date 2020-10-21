/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_GPIO_DIGITALOUTPUT_H
#define _ESP_GPIO_DIGITALOUTPUT_H

#include "library.h"
#include "Pin.h"

F_BEGIN_NAMESPACE

class DigitalOutput : public Pin
{
public:
    DigitalOutput(gpio_num_t pin);
    virtual ~DigitalOutput() {}

    void setLevel(bool state);
    void setHigh() { setLevel(true); };
    void setLow() { setLevel(false); };
};

F_END_NAMESPACE

#endif // _ESP_GPIO_DIGITALOUTPUT_H