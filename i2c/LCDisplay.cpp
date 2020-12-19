/**
 * ESP/Native GPIO Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "LCDisplay.h"

#include "esp_err.h"
#include "driver/i2c.h"
#include "rom/ets_sys.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdarg>

F_USE_NAMESPACE

static const uint32_t LCD_RS = 1;
static const uint32_t LCD_RW = 2;
static const uint32_t LCD_E = 4;
static const uint32_t LCD_BK = 8;

char _printBuffer[128];

LCDisplay::LCDisplay(display_type type /* = Type_1602 */, uint8_t i2cAddress /* = 0x27 */) :
    _type(type),
    _i2cAddress(i2cAddress)
{
}

bool LCDisplay::begin(int sdaPin, int sclPin, int i2cPort /* = 0 */)
{
    if (!_port.open(sdaPin, sclPin, i2cPort)) {
        return false;
    }

    initialize();
    return true;
}

void LCDisplay::initialize()
{
    // see data sheet, page 46
    // https://www.sparkfun.com/datasheets/LCD/HD44780.pdf

    uint8_t buffer0[] {
        0x30 + LCD_E,
        0x30
    };

    vTaskDelay(20 / portTICK_PERIOD_MS);
    _port.write(_i2cAddress, buffer0, 2);

    vTaskDelay(5 / portTICK_PERIOD_MS);
    _port.write(_i2cAddress, buffer0, 2);

    vTaskDelay(1 / portTICK_PERIOD_MS);
    _port.write(_i2cAddress, buffer0, 2);

    uint8_t buffer1[] {
        0x20 + LCD_E, // function set, to 4-bit
        0x20
    };
    _port.write(_i2cAddress, buffer1, 12);

    writeRegister(0x2c); // function set
    writeRegister(0x0c); // display on, cursor off
    writeRegister(0x01); // display clear
    writeRegister(0x04); // entry mode set
}

void LCDisplay::clear()
{
    writeRegister(ClearDisplay);
}

void LCDisplay::home()
{
    writeRegister(ReturnHome);
}

void LCDisplay::setPosition(uint8_t address)
{
    writeRegister(DDRAMAddress | (address & 0x7f));
}

void LCDisplay::setPosition(uint8_t row, uint8_t col)
{
    writeRegister(DDRAMAddress | _getAddress(row, col));
}

void LCDisplay::print(const char* pText)
{
    writeData((uint8_t*)pText, strlen(pText));
}

void LCDisplay::print(uint8_t row, uint8_t col, const char* pText)
{
    setPosition(row, col);
    writeData((uint8_t*)pText, std::min(columns() - col, strlen(pText)));
}

void LCDisplay::printf(const char* pFormat, ...)
{
    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_printBuffer, pFormat, argptr);
    writeData((uint8_t*)_printBuffer, strlen(_printBuffer));
}

void LCDisplay::printf(uint8_t row, uint8_t col, const char* pFormat, ...)
{
    setPosition(row, col);

    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_printBuffer, pFormat, argptr);
    writeData((uint8_t*)_printBuffer, std::min(columns() - col, strlen(_printBuffer)));
}

void LCDisplay::setDisplayEnabled(bool enabled)
{
    _displayEnabled = enabled;
    _writeDisplayControl();
}

void LCDisplay::setUnderlineCursorEnabled(bool enabled)
{
    _underlineCursor = enabled;
    _writeDisplayControl();
}

void LCDisplay::setBlockCursorEnabled(bool enabled)
{
    _blockCursor = enabled;
    _writeDisplayControl();
}

void LCDisplay::setBacklight(bool state) 
{
    _backlightEnabled = state;
    _writeDisplayControl();
}

void LCDisplay::writeRegister(uint8_t value)
{
    uint32_t flags = _backlightEnabled ? LCD_BK : 0;
    uint32_t MSN = (value & 0xf0) | flags;
    uint32_t LSN = ((value & 0x0f) << 4) | flags;

    uint8_t buffer[4];
    buffer[0] = MSN | LCD_E;
    buffer[1] = MSN;
    buffer[2] = LSN | LCD_E;
    buffer[3] = LSN;

    _port.write(_i2cAddress, buffer, 4);
}

void LCDisplay::writeData(uint8_t* pData, size_t length)
{
    size_t bufSize = length * 4;
    _buffer.resize(bufSize);

    for (size_t i = 0; i < length; ++i) {
        uint32_t flags = LCD_RS | (_backlightEnabled ? LCD_BK : 0);
        uint32_t MSN = (pData[i] & 0xf0) | flags;
        uint32_t LSN = ((pData[i] & 0x0f) << 4) | flags;

        size_t j = i * 4;
        _buffer[j  ] = MSN | LCD_E;
        _buffer[j+1] = MSN;
        _buffer[j+2] = LSN | LCD_E;
        _buffer[j+3] = LSN;
    }

    _port.write(_i2cAddress, _buffer.data(), bufSize);
}

uint8_t LCDisplay::_getAddress(uint8_t row, uint8_t col)
{
    switch(row) {
        case 0: return col;
        case 1: return 64 + col;
        case 2: return 20 + col;
        case 3: return 84 + col;
    }

    return 0;
}

void LCDisplay::_writeDisplayControl()
{
    uint8_t flags =
        (_displayEnabled ? DC_Display : 0) |
        (_underlineCursor ? DC_Cursor : 0) |
        (_blockCursor ? DC_Blink : 0);

    writeRegister(DisplayControl | flags);
}