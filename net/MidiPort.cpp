/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "MidiPort.h"
#include "MidiController.h"

F_USE_NAMESPACE

MidiPort::MidiPort() :
    _pListener(nullptr),
    _mutex(xSemaphoreCreateMutex())
{
}

void MidiPort::dispatch()
{
    xSemaphoreTake(_mutex, portMAX_DELAY);

    while(!_messageQueue.empty()) {
        MidiMessage message = _messageQueue.front();
        _messageQueue.pop();
        dispatchMessage(message);
    }

    while(!_sysExQueue.empty()) {
        std::string sysExMessage = _sysExQueue.front();
        _sysExQueue.pop();
        dispatchSysEx(sysExMessage);
    }

    xSemaphoreGive(_mutex);
}

void MidiPort::setListener(MidiListener* pListener)
{
    _pListener = pListener;
}

void MidiPort::dispatchMessage(const MidiMessage& message)
{
    uint8_t status = message.status();
    uint8_t data0 = message.data0();
    uint8_t data1 = message.data1();

    if (status == MidiStatus::ControlChange) {
        switch(data0) {
            case MidiController::RPN_MSB:
                _rpn_param = (_nrpn_param & 0x7f) | (data1 << 7);
                _nrpn_param = 0x8000;
                return;
            case MidiController::RPN_LSB:
                _rpn_param = (_nrpn_param & 0x3f80) | data1;
                _nrpn_param = 0x8000;
                return;

            case MidiController::NRPN_MSB:
                _nrpn_param = (_nrpn_param & 0x7f) | (data1 << 7);
                _rpn_param = 0x8000;
                return;
            case MidiController::NRPN_LSB:
                _nrpn_param = (_nrpn_param & 0x3f80) | data1;
                _rpn_param = 0x8000;
                return;

            case MidiController::DataEntry_MSB:
                _param_value = (_param_value & 0x7f) | (data1 << 7);
                if (_pListener) {
                    if (_rpn_param < 0x8000) {
                        _pListener->onRPN(_rpn_param, _param_value);
                    }
                    else if (_nrpn_param < 0x8000) {
                        _pListener->onNRPN(_nrpn_param, _param_value);
                    }
                }
                return;
            case MidiController::DataEntry_LSB:
                _param_value = (_param_value & 0x3f80) | data1;
                if (_pListener) {
                    if (_rpn_param < 0x8000) {
                        _pListener->onRPN(_rpn_param, _param_value);
                    }
                    else if (_nrpn_param < 0x8000) {
                        _pListener->onNRPN(_nrpn_param, _param_value);
                    }
                }
                return;
        }
    }

    if (_pListener) {
        _pListener->onMidiMessage(message);
    }
}

void MidiPort::enqueueMessage(const MidiMessage& message)
{
    xSemaphoreTake(_mutex, portMAX_DELAY);

    while(_messageQueue.size() > QUEUE_MAX_SIZE) {
        _messageQueue.pop();
    }

    _messageQueue.emplace(message);

    xSemaphoreGive(_mutex);
}

void MidiPort::dispatchSysEx(const std::string& sysEx)
{
    if (_pListener) {
        _pListener->onSysEx(sysEx);
    }
}

void MidiPort::enqueueSysEx(const std::string& sysEx)
{
    xSemaphoreTake(_mutex, portMAX_DELAY);

    while(_sysExQueue.size() > QUEUE_MAX_SIZE) {
        _sysExQueue.pop();
    }

    _sysExQueue.emplace(sysEx);

    xSemaphoreGive(_mutex);
}