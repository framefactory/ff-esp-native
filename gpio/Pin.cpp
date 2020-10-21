/**
 * ESP/Arduino  Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Pin.h"

F_USE_NAMESPACE

Pin::Pin(gpio_num_t pin, gpio_mode_t direction) :
    _pin(pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, direction);
}


void Pin::setInverted(bool state)
{
    _isInverted = state;
}
