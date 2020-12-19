/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Pin.h"

F_USE_NAMESPACE


Pin::Pin(int pin, gpio_mode_t direction)
{
    if (pin < 0) {
        return;
    }

    _pin = static_cast<gpio_num_t>(pin);

    if (direction == GPIO_MODE_INPUT && !GPIO_IS_VALID_GPIO(_pin)) {
        printf("[Pin::constructor] WARNING pin #%d not valid as input", pin);
        return;
    }
    if (direction != GPIO_MODE_INPUT && !GPIO_IS_VALID_OUTPUT_GPIO(_pin)) {
        printf("[Pin::constructor] WARNING pin #%d not valid as output", pin);
        return;
    }

    gpio_reset_pin(_pin);
    gpio_set_direction(_pin, direction);
    _isValid = true;
}

Pin::~Pin()
{
    if (isValid()) {
        gpio_reset_pin(_pin);
    }

    _isValid = false;
}


void Pin::setInverted(bool state)
{
    _isInverted = state;
}
