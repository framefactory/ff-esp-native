/**
 * ESP/Native GPIO Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_I2CPORT_H
#define _ESP_NATIVE_I2CPORT_H

#include "../library.h"

F_BEGIN_NAMESPACE

class I2CPort
{
    F_DISABLE_COPY(I2CPort);

public:
    I2CPort() {}
    ~I2CPort() { close(); }

    bool open(int sdaPin, int sclPin, int i2cPort = 0);
    void close();

    void write(uint8_t address, uint8_t* pData, size_t length);

    void setClockSpeed(uint32_t speed);
    void setPullupsEnabled(bool enabled);

    bool isOpen() const { return _isOpen; }

    uint32_t clockSpeeed() const { return _clockSpeed; }
    bool pullupsEnabled() const { return _pullupsEnabled; }

private:
    bool _isOpen = false;
    int _port = 0;
    uint32_t _clockSpeed = 100000;
    bool _pullupsEnabled = true;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_I2CPORT_H