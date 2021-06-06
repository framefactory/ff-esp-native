/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_MIDIMESSAGE_H
#define _ESP_NATIVE_NET_MIDIMESSAGE_H

#include "../library.h"
#include "MidiStatus.h"

#include <string>

F_BEGIN_NAMESPACE

class MidiMessage
{
public:
    /// Returns the number of bytes in a MIDI message based on the given status.
    static size_t lengthFromByte0(uint8_t byte0);

    /// Creates a new MIDI message with the given bytes.
    MidiMessage(uint8_t status = 0, uint8_t data0 = 0, uint8_t data1 = 0);

    /// Sets the message content.
    void setBytes(uint8_t status, uint8_t data0 = 0, uint8_t data1 = 0);

    /// Returns true if this is a channel message.
    bool isChannelMessage() const { return _bytes[0] < 0xf0; }
    /// Returns true if this is a system realtime message.
    bool isSystemRealtime() const { return _bytes[0] & 0xf8; }

    uint8_t status() const { return _bytes[0] < 0xf0 ? _bytes[0] & 0xf0 : _bytes[0]; }
    uint8_t channel() const { return _bytes[0] & 0x0f; }

    uint8_t data0() const { return _bytes[1]; }
    uint8_t data1() const { return _bytes[2]; }
    uint8_t note() const { return data0(); }
    uint8_t program() const { return data0(); }
    uint8_t controller() const { return data0(); }
    uint8_t velocity() const { return data1(); }
    uint8_t value() const { return data1(); }
    size_t size() const { return lengthFromByte0(_bytes[0]); }

    std::string toString() const;
    operator std::string() const { return toString(); }

private:
    uint8_t _bytes[3];
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_MIDIMESSAGE_H