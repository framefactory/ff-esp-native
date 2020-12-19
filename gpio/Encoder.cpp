/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "Encoder.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

F_USE_NAMESPACE

void _buttonHandler(void* pData)
{
    Encoder* pEncoder = static_cast<Encoder*>(pData);
    pEncoder->onButton();
}

void _s1_ISR(void* pData)
{
    Encoder* pEncoder = static_cast<Encoder*>(pData);
    pEncoder->onS1_ISR();
}

void _s1_TMR(void* pData)
{
    Encoder* pEncoder = static_cast<Encoder*>(pData);
    pEncoder->onS1_TMR();
}

void _s2_ISR(void* pData)
{
    Encoder* pEncoder = static_cast<Encoder*>(pData);
    pEncoder->onS2_ISR();
}

void _s2_TMR(void* pData)
{
    Encoder* pEncoder = static_cast<Encoder*>(pData);
    pEncoder->onS2_TMR();
}

Encoder::Encoder(int button, int s1, int s2, bool invertState) :
    _btnPin(button),
    _s1Pin(s1),
    _s2Pin(s2),
    _btnState(0),
    _s1State(0),
    _s1PrevState(0),
    _s2State(0),
    _s2PrevState(0),
    _pushCount(0),
    _isPressed(false),
    _delta(0),
    _invertState(invertState)
{
    gpio_reset_pin((gpio_num_t)button);
    gpio_set_direction((gpio_num_t)button, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)button, GPIO_FLOATING);
    gpio_set_intr_type((gpio_num_t)button, GPIO_INTR_ANYEDGE);
    gpio_intr_enable((gpio_num_t)button);

    gpio_reset_pin((gpio_num_t)s1);
    gpio_set_direction((gpio_num_t)s1, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)s1, GPIO_FLOATING);
    gpio_set_intr_type((gpio_num_t)s1, GPIO_INTR_ANYEDGE);
    gpio_intr_enable((gpio_num_t)s1);

    gpio_reset_pin((gpio_num_t)s2);
    gpio_set_direction((gpio_num_t)s2, GPIO_MODE_INPUT);
    gpio_set_pull_mode((gpio_num_t)s2, GPIO_FLOATING);
    gpio_set_intr_type((gpio_num_t)s2, GPIO_INTR_ANYEDGE);
    gpio_intr_enable((gpio_num_t)s2);

    int flags = 0; //ESP_INTR_FLAG_EDGE;
    gpio_install_isr_service(flags);

    gpio_isr_handler_add((gpio_num_t)button, &_buttonHandler, this);
    gpio_isr_handler_add((gpio_num_t)s1, &_s1_ISR, this);
    gpio_isr_handler_add((gpio_num_t)s2, &_s2_ISR, this);

    // TIMERS
    esp_timer_init();

    esp_timer_create_args_t args1 {
        .callback = &_s1_TMR,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "S1"
    };

    esp_timer_create(&args1, &_s1TimerHandle);

    esp_timer_create_args_t args2 {
        .callback = &_s2_TMR,
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "S2"
    };

    esp_timer_create(&args2, &_s2TimerHandle);
}

Encoder::~Encoder()
{
    gpio_reset_pin((gpio_num_t)_btnPin);
    gpio_isr_handler_remove((gpio_num_t)_btnPin);

    gpio_reset_pin((gpio_num_t)_s1Pin);
    gpio_isr_handler_remove((gpio_num_t)_s1Pin);

    gpio_reset_pin((gpio_num_t)_s2Pin);
    //gpio_isr_handler_remove((gpio_num_t)_s2Pin);
}

void Encoder::onButton()
{
    int state = gpio_get_level((gpio_num_t)_btnPin);
    state = _invertState ? 1 - state : state;

    _isPressed = state > 0;

    if (_btnState == 1 && state == 0) {
        _pushCount++;
        _btnState = state;
    }
}

void Encoder::onS1_ISR()
{
    esp_timer_stop(_s1TimerHandle);
    esp_timer_start_once(_s1TimerHandle, 150);
}

void Encoder::onS1_TMR()
{
    esp_timer_stop(_s1TimerHandle);

    int s1 = gpio_get_level((gpio_num_t)_s1Pin);
    s1 = _invertState ? 1 - s1 : s1;
    _s1State = s1;

    if (_s1State == 0) {
        if (_s2State == 0) {
            _delta++;
        }
        else {
            _delta--;
        }
    }
}

void Encoder::onS2_ISR()
{
    esp_timer_stop(_s2TimerHandle);
    esp_timer_start_once(_s2TimerHandle, 150);
}

void Encoder::onS2_TMR()
{
    esp_timer_stop(_s1TimerHandle);

    int s2 = gpio_get_level((gpio_num_t)_s2Pin);
    s2 = _invertState ? 1 - s2 : s2;
    _s2State = s2;
}
