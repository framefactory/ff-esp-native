/**
 * ESP/Native GPIO Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_LCDISPLAY_H
#define _ESP_NATIVE_LCDISPLAY_H

#include "../library.h"
#include "I2CPort.h"
#include <vector>

F_BEGIN_NAMESPACE

class LCDisplay
{
    F_DISABLE_COPY(LCDisplay);

public:
    enum display_type {
        Type_1602,
        Type_2004
    };

    enum register_type : uint8_t {
        NoOp           = 0x00,
        ClearDisplay   = 0x01,
        ReturnHome     = 0x02,
        EntryMode      = 0x04,
        DisplayControl = 0x08,
        MoveShift      = 0x10,
        FunctionSet    = 0x20,
        CGRAMAddress   = 0x40,
        DDRAMAddress   = 0x80
    };

    enum register_flag : uint8_t {
        EM_Shift       = 1,
        EM_Increment   = 2,
        DC_Blink       = 1,
        DC_Cursor      = 2,
        DC_Display     = 4,
        MS_Right       = 4,
        MS_Shift       = 8,
        FS_5x10Font    = 4,
        FS_2Lines      = 8,
        FS_8Bits       = 16
    };

    LCDisplay(display_type type = Type_1602, uint8_t i2cAddress = 0x27);

    bool begin(int sdaPin, int sclPin, int i2cPort = 0);

    void initialize();

    void clear();
    void home();
    void setPosition(uint8_t address);
    void setPosition(uint8_t row, uint8_t col);

    void print(const char* pText);
    void print(uint8_t row, uint8_t col, const char* pText);
    void printf(const char* pFormat, ...);
    void printf(uint8_t row, uint8_t col, const char* pFormat, ...);

    void setDirection();
    void setShift();
    void setDisplayEnabled(bool enabled);
    void setUnderlineCursorEnabled(bool enabled);
    void setBlockCursorEnabled(bool enabled);
    void setBacklight(bool state);

    void writeRegister(uint8_t value);
    void writeData(uint8_t* pData, size_t length);

    size_t columns() const { return _type == Type_1602 ? 16 : 20; }
    size_t rows() const { return _type == Type_1602 ? 2 : 4; }

private:
    uint8_t _getAddress(uint8_t row, uint8_t col);
    void _writeDisplayControl();

    I2CPort _port;
    display_type _type;
    uint8_t _i2cAddress;
    std::vector<uint8_t> _buffer;
    bool _backlightEnabled = true;
    bool _displayEnabled = true;
    bool _underlineCursor = false;
    bool _blockCursor = false;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_LCDISPLAY_H