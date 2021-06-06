/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiStatus.h"

F_USE_NAMESPACE

const char* MidiStatus::name(uint8_t status)
{
    switch(status) {
        case NoteOff: return "Note Off";
        case NoteOn: return "Note On";
        case KeyPressure: return "Key Pressure";
        case ControlChange: return "Control Change";
        case ProgramChange: return "Program Change";
        case ChannelPressure: return "Channel Pressure";
        case PitchBend: return "Pitch Bend";

        case SystemExclusive: return "System Exclusive";
        case MTCQuarterFrame: return "MTC Quarter Frame";
        case SongPosition: return "Song Position Pointer";
        case SongSelect: return "Song Select";
        case TuneRequest: return "Tune Request";
        case EndOfExclusive: return "End of Exclusive";

        case TimingClock: return "Timing Clock";
        case 0b11111001: return "Undefined 11111001";
        case Start: return "Start";
        case Continue: return "Continue";
        case Stop: return "Stop";
        case 0b11111101: return "Undefined 11111101";
        case ActiveSensing: return "Active Sensing";
        case Reset: return "System Reset";
    }

    return "Undefined";
}