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
    _i2cAddress(i2cAddress),
    _entryModeFlags(EM_Increment),
    _displayControlFlags(DC_Display),
    _backlightEnabled(true)
{
}

bool LCDisplay::open(int sdaPin, int sclPin, int i2cPort /* = 0 */)
{
    if (!_port.open(sdaPin, sclPin, i2cPort)) {
        return false;
    }

    initialize();
    return true;
}

void LCDisplay::close()
{
    _port.close();
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

void LCDisplay::moveLeft()
{
    writeRegister(MoveShift);
}

void LCDisplay::moveRight()
{
    writeRegister(MoveShift | MS_Right);
}

void LCDisplay::shiftLeft()
{
    writeRegister(MoveShift | MS_Shift);
}

void LCDisplay::shiftRight()
{
    writeRegister(MoveShift | MS_Shift | MS_Right);
}

void LCDisplay::print(const char* pText)
{
    writeDisplay((uint8_t*)pText, strlen(pText));
}

void LCDisplay::print(uint8_t row, uint8_t col, const char* pText)
{
    setPosition(row, col);
    writeDisplay((uint8_t*)pText, std::min(columns() - col, strlen(pText)));
}

void LCDisplay::printf(const char* pFormat, ...)
{
    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_printBuffer, pFormat, argptr);
    writeDisplay((uint8_t*)_printBuffer, strlen(_printBuffer));
}

void LCDisplay::printf(uint8_t row, uint8_t col, const char* pFormat, ...)
{
    setPosition(row, col);

    va_list argptr;
    va_start(argptr, pFormat);
    vsprintf(_printBuffer, pFormat, argptr);
    writeDisplay((uint8_t*)_printBuffer, std::min(columns() - col, strlen(_printBuffer)));
}

void LCDisplay::setEntryDirection(bool state)
{
    _entryModeFlags = state ?
        _entryModeFlags | EM_Increment :
        _entryModeFlags & ~EM_Increment;

    writeRegister(EntryMode | _entryModeFlags);
}

void LCDisplay::enableShift(bool state)
{
    _entryModeFlags = state ?
        _entryModeFlags | EM_Shift :
        _entryModeFlags & ~EM_Shift;

    writeRegister(EntryMode | _entryModeFlags);
}

void LCDisplay::enableDisplay(bool state)
{
    _displayControlFlags = state ?
        _displayControlFlags | DC_Display :
        _displayControlFlags & ~DC_Display;
    
    writeRegister(DisplayControl | _displayControlFlags);
}

void LCDisplay::showUnderlineCursor(bool state)
{
    _displayControlFlags = state ?
        _displayControlFlags | DC_Underline :
        _displayControlFlags & ~DC_Underline;
    
    writeRegister(DisplayControl | _displayControlFlags);
}

void LCDisplay::showBlockCursor(bool state)
{
    _displayControlFlags = state ?
        _displayControlFlags | DC_Block :
        _displayControlFlags & ~DC_Block;
    
    writeRegister(DisplayControl | _displayControlFlags);
}

void LCDisplay::enableBacklight(bool state) 
{
    _backlightEnabled = state;
    writeRegister(NoOp);
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

void LCDisplay::writeDisplay(uint8_t* pData, size_t length)
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

void LCDisplay::writeCharacter(uint8_t index, uint8_t* pData)
{
    if (index < 8) {
        uint8_t address = index * 8;
        writeRegister(CGRAMAddress | address);
        writeDisplay(pData, 8);
    }
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
