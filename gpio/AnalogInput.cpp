/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "AnalogInput.h"

F_USE_NAMESPACE

AnalogInput::AnalogInput(int pin) :
    Pin(pin, GPIO_MODE_INPUT)
{
}

void AnalogInput::setPullMode(gpio_pull_mode_t mode)
{
    gpio_set_pull_mode(pin(), mode);
}