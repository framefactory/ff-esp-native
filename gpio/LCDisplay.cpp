/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "LCDisplay.h"


#include <cstring>
#include <cstdio>
#include <cstdarg>

F_USE_NAMESPACE

char _buffer[256];

LCDisplayUART::LCDisplayUART(int pin, uart_port_t uart) :
    _pin(pin),
    _uart(uart)
{
}

bool LCDisplayUART::connect()
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .use_ref_tick = false,
    };

    ESP_ERROR_CHECK(uart_param_config(_uart, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(_uart, _pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(_uart,  256, 512, 0, nullptr, 0));

    return true;
}

bool LCDisplayUART::disconnect()
{
    return true;
}

void LCDisplayUART::writeBytes(const char* pBytes, size_t length)
{
    uart_write_bytes(_uart, pBytes, length);
}

void LCDisplayUART::print(const char* pText)
{
    uart_write_bytes(_uart, pText, strlen(pText));
}

void LCDisplayUART::printf(const char* pFormat, ...)
{
    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_buffer, pFormat, argptr);
    uart_write_bytes(_uart, _buffer, strlen(_buffer));
}

void LCDisplayUART::printf(int row, int column, const char* pFormat, ...)
{
    setPosition(row, column);

    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_buffer, pFormat, argptr);
    uart_write_bytes(_uart, _buffer, strlen(_buffer));
}

void LCDisplayUART::print(const std::string& text)
{
    const char* pData = text.c_str();
    uart_write_bytes(_uart, pData, text.size());
}

void LCDisplayUART::print(int row, int column, const std::string& text)
{
    setPosition(row, column);

    const char* pData = text.c_str();
    uart_write_bytes(_uart, pData, text.size());
}

void LCDisplayUART::on()
{
    const char pData[] = { 0xFE, 0x0C };
    uart_write_bytes(_uart, pData, 2);
}

void LCDisplayUART::off()
{
    const char pData[] = { 0xFE, 0x08 };
    uart_write_bytes(_uart, pData, 2);
}

void LCDisplayUART::clear()
{
    const char pData[] = { 0xFE, 0x01 };
    uart_write_bytes(_uart, pData, 2);
}

void LCDisplayUART::setPosition(int row, int column)
{
    char pos = 0x80 + row * 64 + column;
    const char pData[] = { 0xFE, pos };
    uart_write_bytes(_uart, pData, 2);
}

void LCDisplayUART::showBlinkingCursor()
{
    const char pData[] = { 0xFE, 0x0D };
    uart_write_bytes(_uart, pData, 2);
}

void LCDisplayUART::showUnderlineCursor()
{
    const char pData[] = { 0xFE, 0x0E };
    uart_write_bytes(_uart, pData, 2);
}
