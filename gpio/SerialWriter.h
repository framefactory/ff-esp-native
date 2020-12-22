/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_SERIALWRITER_H
#define _ESP_NATIVE_SERIALWRITER_H

#include "../library.h"
#include "../gpio/DigitalOutput.h"

#include "esp32/rom/ets_sys.h"

F_BEGIN_NAMESPACE

class SerialWriter
{
public:
    SerialWriter(int dataPin, int clockPin, int latchPin = -1);
    virtual ~SerialWriter() {}

    void writeByte(uint8_t byte, bool lsbFirst = false);
    void writeBit(bool level);
    void writeStart();
    void writeStop();
    void clock();
    void latch();

    void setDelay(uint32_t delayMicroseconds);
    void setInverted(bool dataPin, bool clockPin = false, bool latchPin = false);

private:
    DigitalOutput _dataPin;
    DigitalOutput _clockPin;
    DigitalOutput _latchPin;
    uint32_t _delayMicroseconds;
};

inline void SerialWriter::writeBit(bool level)
{
    _dataPin.setLevel(level);
    ets_delay_us(_delayMicroseconds);
    clock();
}

inline void SerialWriter::writeStart()
{
    _dataPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _clockPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _dataPin.setLow();
    ets_delay_us(_delayMicroseconds);
    _clockPin.setLow();
}

inline void SerialWriter::writeStop()
{
    _dataPin.setLow();
    ets_delay_us(_delayMicroseconds);
    _clockPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _dataPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _clockPin.setLow();
}

inline void SerialWriter::clock()
{
    _clockPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _clockPin.setLow();
}

inline void SerialWriter::latch()
{
    _latchPin.setHigh();
    ets_delay_us(_delayMicroseconds);
    _latchPin.setLow();
}

F_END_NAMESPACE

#endif // _ESP_NATIVE_SERIALWRITER_H