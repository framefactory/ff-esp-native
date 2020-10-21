/**
 * ESP/Arduino GPIO Library
 * Copyright 2020 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "DigitalInput.h"

F_USE_NAMESPACE

DigitalInput::DigitalInput(gpio_num_t pin) :
    Pin(pin, GPIO_MODE_INPUT),
    _level(false),
    _interruptType(GPIO_INTR_DISABLE),
    _debounceDelay(100),
    _debounceTimer(nullptr)
{
}

DigitalInput::~DigitalInput()
{
    gpio_intr_disable(pin());
    gpio_isr_handler_remove(pin());

    esp_timer_stop(_debounceTimer);
    esp_timer_delete(_debounceTimer);
}

void DigitalInput::setPull(gpio_pull_mode_t pull)
{
    gpio_set_pull_mode(pin(), pull);
}

void DigitalInput::enableInterrupt(gpio_int_type_t type, uint64_t usecsDebounce)
{
    _interruptType = type;
    gpio_set_intr_type(pin(), type);
    gpio_isr_handler_add(pin(), &DigitalInput::_interruptHandler, this);
    gpio_intr_enable(pin());

    _debounceDelay = usecsDebounce;
    if (usecsDebounce > 0)
        esp_timer_init();

        esp_timer_create_args_t args1 {
            .callback = &DigitalInput::_timerHandler,
            .arg = this,
            .dispatch_method = ESP_TIMER_TASK,
            .name = "Debounce"
        };

        esp_timer_create(&args1, &_debounceTimer);
    }
}

bool DigitalInput::level() const
{
    if (_interruptType == GPIO_INTR_DISABLE) {
        _level = gpio_get_level(pin()) > 0;
    }

    return isInverted() ? !_level : _level; 
}

void DigitalInput::onChange(bool level)
{
}

void DigitalInput::_interruptHandler(void* arg)
{
    DigitalInput* pPin = static_cast<DigitalInput*>(arg);

    if (pPin->_debounceEnabled) {
        esp_timer_stop(pPin->_debounceTimer);
        esp_timer_start_once(pPin->_debounceTimer, pPin->_debounceDelay);
    }
    else {
        _level = gpio_get_level(pPin->pin()) > 0;
        pPin->onChange(_level);
    }
}

void DigitalInput::_timerHandler(void* arg)
{
    DigitalInput* pPin = static_cast<DigitalInput*>(arg);

    esp_timer_stop(pPin->_debounceTimer);

    _level = gpio_get_level(pPin->pin()) > 0;
    pPin->onChange(_level);
}