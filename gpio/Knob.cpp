/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Knob.h"
#include "ADC.h"

#include "driver/gpio.h"
#include "driver/adc.h"

#include <algorithm>

F_USE_NAMESPACE

const int Knob::RAW_MIN = 130;
const int Knob::RAW_MAX = 3880;

Knob::Knob(uint32_t pin) :
    _pin(pin),
    _prevRaw(0)
{
    uint32_t adc = ADC::gpio2adc(_pin);

    if (adc == 0) {
        adc_channel_t channel = (adc_channel_t)ADC::gpio2channel(_pin);
        adc_gpio_init(ADC_UNIT_1, channel);
    }
}

float Knob::value() const
{
    int raw = std::min(RAW_MAX, std::max(rawValue(), RAW_MIN));

    float val = (raw - RAW_MIN) / float(RAW_MAX - RAW_MIN);
    return val;
}

int Knob::rawValue() const
{
    uint32_t adc = ADC::gpio2adc(_pin);
    uint32_t channel = ADC::gpio2channel(_pin);

    int value = 0;
    
    if (adc == 0) {
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten((adc1_channel_t)channel, ADC_ATTEN_DB_11);
        for (int i = 0; i < 16; ++i) {
            value += adc1_get_raw((adc1_channel_t)channel);
        }
    }
    else {
        esp_err_t result = adc2_config_channel_atten((adc2_channel_t)channel, ADC_ATTEN_DB_11);
        if (result != ESP_OK) {
            return 0;
        }

        for (int i = 0; i < 16; ++i) {
            int v;
            adc2_get_raw((adc2_channel_t)channel, ADC_WIDTH_BIT_12, &v);
            value += v;
        }
    }

    value >>= 4;

    if (abs(value - _prevRaw) < 4) {
        return _prevRaw;
    }

    _prevRaw = value;
    return value;
}