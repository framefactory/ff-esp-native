/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "DigitalOutput.h"

F_USE_NAMESPACE

DigitalOutput::DigitalOutput(int pin) :
    Pin(pin, GPIO_MODE_OUTPUT)
{
}
