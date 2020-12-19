/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_ENCODER_H
#define _ESP_NATIVE_ENCODER_H

#include "../library.h"
#include "esp_timer.h"

F_BEGIN_NAMESPACE

class Encoder
{
public:
    Encoder(int button, int s1, int s2, bool invertState = false);
    virtual ~Encoder();

    void onButton();
    void onS1_ISR();
    void onS1_TMR();
    void onS2_ISR();
    void onS2_TMR();

    bool isPressed() const { return _isPressed; }
    int delta() const { return _delta; }

private:
    int _btnPin;
    int _s1Pin;
    int _s2Pin;

    int _btnState;
    int _s1State;
    int _s1PrevState;
    int _s2State;
    int _s2PrevState;

    int _pushCount;
    bool _isPressed;
    int _delta;

    bool _invertState;

    esp_timer_handle_t _s1TimerHandle;
    esp_timer_handle_t _s2TimerHandle;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_ENCODER_H