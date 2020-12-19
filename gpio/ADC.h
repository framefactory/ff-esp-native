/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_ADC_H
#define _ESP_NATIVE_ADC_H

#include "library.h"

F_BEGIN_NAMESPACE

class ADC
{
private:
    ADC() {};
 
public:
    static uint32_t gpio2adc(uint32_t gpio);
    static uint32_t gpio2channel(uint32_t gpio);
    static uint32_t channel2gpio(uint32_t adc, uint32_t channel);
    
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_ADC_H