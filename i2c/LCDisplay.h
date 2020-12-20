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
        DC_Block       = 1,
        DC_Underline   = 2,
        DC_Display     = 4,
        MS_Right       = 4,
        MS_Shift       = 8,
        FS_5x10Font    = 4,
        FS_2Lines      = 8,
        FS_8Bits       = 16
    };

    /// Creates a new LCD Display of the given type, reachable at the given I2C address.
    LCDisplay(display_type type = Type_1602, uint8_t i2cAddress = 0x27);

    /// Establishes I2C on the given GPIO pins and initializes the display.
    bool open(int sdaPin, int sclPin, int i2cPort = 0);
    /// Ends communication with the display and frees the I2C port resources.
    void close();

    /// Resets and initializes the display.
    /// You shouldn't need to call this explicitly.
    void initialize();

    /// Clears the display.
    void clear();
    /// Clears shift and moves cursor to top left position.
    void home();
    /// Sets cursor position to given raw address.
    void setPosition(uint8_t address);
    /// Sets cursor position to given row and column.
    void setPosition(uint8_t row, uint8_t col);
    /// Moves cursor to the left.
    void moveLeft();
    /// Moves cursor to the right.
    void moveRight();
    /// Shifts display content to the left.
    void shiftLeft();
    /// Shifts display content to the right.
    void shiftRight();

    /// Prints text at the current position.
    void print(const char* pText);
    /// Prints text at the given position.
    void print(uint8_t row, uint8_t col, const char* pText);
    /// Prints formatted text at the current position.
    void printf(const char* pFormat, ...);
    /// Prints formatted text at the given position.
    void printf(uint8_t row, uint8_t col, const char* pFormat, ...);

    /// Writing display data increments (true) or decrements (false) cursor position. True (increment) by default.
    void setEntryDirection(bool state);
    /// Activates shift. Off by default.
    void enableShift(bool state);
    /// Activates the display. On by default.
    void enableDisplay(bool state);
    /// Shows an underline cursor at current position. Off by default.
    void showUnderlineCursor(bool state);
    /// Shows a blinking block cursor at current position. Off by default.
    void showBlockCursor(bool state);
    /// Activates the LED background light. On by default.
    void enableBacklight(bool state);

    /// Write register byte.
    void writeRegister(uint8_t value);
    /// Write data to display RAM. 
    void writeDisplay(uint8_t* pData, size_t length);
    /// Write 8 rows of bitmap data for custom 5x8 character at index (0 - 7).
    void writeCharacter(uint8_t index, uint8_t* pData);

    size_t columns() const { return _type == Type_1602 ? 16 : 20; }
    size_t rows() const { return _type == Type_1602 ? 2 : 4; }

private:
    uint8_t _getAddress(uint8_t row, uint8_t col);

    I2CPort _port;
    display_type _type;
    uint8_t _i2cAddress;
    uint8_t _entryModeFlags;
    uint8_t _displayControlFlags;
    std::vector<uint8_t> _buffer;
    bool _backlightEnabled;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_LCDISPLAY_H