/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_LCDISPLAY_H
#define _ESP_NATIVE_LCDISPLAY_H

#include "../library.h"
#include "driver/uart.h"

#include <string>


F_BEGIN_NAMESPACE

class LCDisplayUART
{
public:
    LCDisplayUART(int pin, uart_port_t uart = UART_NUM_2);
    virtual ~LCDisplayUART() {}

    bool connect();
    bool disconnect();

    void writeBytes(const char* pBytes, size_t length);
    void print(const char* pText);
    void printf(const char* pFormat, ...);
    void printf(int row, int column, const char* pFormat, ...);
    void print(const std::string& text);
    void print(int row, int column, const std::string& text);
    void on();
    void off();
    void clear();
    void setPosition(int row, int column);
    void showBlinkingCursor();
    void showUnderlineCursor();

private:
    int _pin;
    uart_port_t _uart;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_LCDISPLAY_H