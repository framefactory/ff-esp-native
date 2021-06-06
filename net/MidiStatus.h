/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_MIDISTATUS_H
#define _ESP_NATIVE_NET_MIDISTATUS_H

#include "../library.h"

F_BEGIN_NAMESPACE

struct MidiStatus
{
    enum status_type : uint8_t {
        NoteOff         = 0b10000000,
        NoteOn          = 0b10010000,
        KeyPressure     = 0b10100000,
        ControlChange   = 0b10110000,
        ProgramChange   = 0b11000000,
        ChannelPressure = 0b11010000,
        PitchBend       = 0b11100000,

        SystemCommon    = 0b11110000,
        SystemExclusive = 0b11110000,
        MTCQuarterFrame = 0b11110001,
        SongPosition    = 0b11110010,
        SongSelect      = 0b11110011,
        TuneRequest     = 0b11110110,
        EndOfExclusive  = 0b11110111,

        SystemRealtime  = 0b11111000,
        TimingClock     = 0b11111000,
        Start           = 0b11111010,
        Continue        = 0b11111011,
        Stop            = 0b11111100,
        ActiveSensing   = 0b11111110,
        Reset           = 0b11111111,
    };

    static const char* name(uint8_t status);

    MidiStatus(uint8_t status = NoteOn) : _value(status) {}

    bool operator==(status_type other) const { return _value == other; }
    bool operator!=(status_type other) const { return _value != other; }

    operator status_type() const { return static_cast<status_type>(_value); }

    const char* name() const { return MidiStatus::name(_value); }

private:
    static const char* _names[];
    uint8_t _value;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_MIDISTATUS_H