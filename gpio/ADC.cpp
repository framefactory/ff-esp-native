/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "ADC.h"

F_USE_NAMESPACE

uint32_t ADC::gpio2adc(uint32_t gpio)
{
    return gpio >= 32 ? 0 : 1;
}

uint32_t ADC::gpio2channel(uint32_t gpio)
{
    switch(gpio) {
        // ADC 1
        case 32: return 4;
        case 33: return 5;
        case 34: return 6;
        case 35: return 7;
        case 36: return 0;
        case 37: return 1;
        case 38: return 2;
        case 39: return 3;

        // ADC 2
        case 0: return 1;
        case 2: return 2;
        case 4: return 0;
        case 12: return 5;
        case 13: return 4;
        case 14: return 6;
        case 15: return 3;
        case 25: return 8;
        case 26: return 9;
        case 27: return 7;
    }

    return 0;
}

uint32_t ADC::channel2gpio(uint32_t adc, uint32_t channel)
{
    if (adc == 0) {
        switch(channel) {
            case 0: return 36;
            case 1: return 37;
            case 2: return 38;
            case 3: return 39;
            case 4: return 32;
            case 5: return 33;
            case 6: return 34;
            case 7: return 35;
        }
    }
    else {
        switch(channel) {
            case 0: return 4;
            case 1: return 0;
            case 2: return 2;
            case 3: return 15;
            case 4: return 13;
            case 5: return 12;
            case 6: return 14;
            case 7: return 27;
            case 8: return 25;
            case 9: return 26;
        }
    }

    return 0;
}