/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_GPIO_KNOB_H
#define _ESP_GPIO_KNOB_H

#include "library.h"

F_BEGIN_NAMESPACE

class Knob
{
public:
    Knob(uint32_t pin);
    virtual ~Knob() {}

    float value() const;
    int rawValue() const;

private:
    static const int RAW_MIN;
    static const int RAW_MAX;

    uint32_t _pin;
    mutable int _prevRaw;
};

F_END_NAMESPACE

#endif // _ESP_GPIO_KNOB_H