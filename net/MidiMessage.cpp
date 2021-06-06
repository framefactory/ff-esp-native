/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiMessage.h"
#include <sstream>

F_USE_NAMESPACE

size_t MidiMessage::lengthFromByte0(uint8_t byte0)
{
    // channel messages
    if (byte0 < 0xf0) {
        uint8_t status = byte0 & 0xf0;
        // program and channel pressure: 2 bytes
        if (status == MidiStatus::ProgramChange || status == MidiStatus::ChannelPressure) {
            return 2;
        }

        // other channel messages: 3 bytes
        return 3;
    }
    // system real time messages, tune request: 1 byte
    if (byte0 & 0x08 || byte0 == MidiStatus::TuneRequest) {
        return 1;
    }
    // MTC, song select: 2 bytes
    if (byte0 == MidiStatus::MTCQuarterFrame || byte0 == MidiStatus::SongSelect) {
        return 2;
    }
    // SPP: 3 bytes
    if (byte0 == MidiStatus::SongPosition) {
        return 3;
    }

    // system exclusive or undefined
    return 0;
}

MidiMessage::MidiMessage(uint8_t status /* = 0 */, uint8_t data0 /* = 0 */, uint8_t data1 /* = 0 */) :
    _bytes { status, data0, data1 }
{
}

void MidiMessage::setBytes(uint8_t status, uint8_t data0 /* = 0 */, uint8_t data1 /* = 0 */)
{
    _bytes[0] = status;
    _bytes[1] = data0;
    _bytes[2] = data1;
}

std::string MidiMessage::toString() const
{
    size_t numBytes = size();
    std::ostringstream oss;

    oss << MidiStatus::name(status());
    if (isChannelMessage()) {
        oss << " (" << (channel() + 1) << ")";
    }

    if (numBytes > 1) {
        oss << ", " << uint32_t(data0());
    }
    if (numBytes > 2) {
        oss << ", " << uint32_t(data1());
    }

    return oss.str();
}