/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "SerialWriter.h"

F_USE_NAMESPACE

SerialWriter::SerialWriter(int dataPin, int clockPin, int latchPin) :
    _dataPin(dataPin),
    _clockPin(clockPin),
    _latchPin(latchPin),
    _delayMicroseconds(1)
{
}

void SerialWriter::writeByte(uint8_t byte, bool lsbFirst /* = false */)
{
    if (lsbFirst) {
        for (uint8_t b = 0; b < 8; ++b) {
            writeBit(byte & (0x01 << b));
        }
    }
    else {
        for (uint8_t b = 0; b < 8; ++b) {
            writeBit(byte & (0x80 >> b));
        }
    }
}

void SerialWriter::setDelay(uint32_t delayMicroseconds)
{
    _delayMicroseconds = delayMicroseconds;
}

void SerialWriter::setInverted(bool dataPin, bool clockPin, bool latchPin)
{
    _dataPin.setInverted(dataPin);
    _clockPin.setInverted(clockPin);
    _latchPin.setInverted(latchPin);
}
