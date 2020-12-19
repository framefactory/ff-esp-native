/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_DIGITALOUTPUT_H
#define _ESP_NATIVE_DIGITALOUTPUT_H

#include "../library.h"
#include "Pin.h"

F_BEGIN_NAMESPACE

class DigitalOutput : public Pin
{
public:
    DigitalOutput(int pin);
    virtual ~DigitalOutput() {}

    void setLevel(bool state) {
        gpio_set_level(pin(), isInverted() ? (state ? 0 : 1) : (state ? 1 : 0));
    }

    void setHigh() { setLevel(true); };
    void setLow() { setLevel(false); };
};


F_END_NAMESPACE

#endif // _ESP_NATIVE_DIGITALOUTPUT_H