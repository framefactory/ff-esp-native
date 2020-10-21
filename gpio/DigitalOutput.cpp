/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "DigitalOutput.h"

F_USE_NAMESPACE

DigitalOutput::DigitalOutput(gpio_num_t pin) :
    Pin(pin, GPIO_MODE_OUTPUT)
{
}

void DigitalOutput::setLevel(bool state)
{
    gpio_set_level(pin(), isInverted() ? (state ? 0 : 1) : (state ? 1 : 0));
}
