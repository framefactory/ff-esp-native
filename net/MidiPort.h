/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_MIDIPORT_H
#define _ESP_NATIVE_NET_MIDIPORT_H

#include "../library.h"
#include "MidiListener.h"
#include "MidiMessage.h"

#include "freertos/semphr.h"
#include <queue>

F_BEGIN_NAMESPACE

class MidiPort
{
public:
    const static size_t QUEUE_MAX_SIZE = 16;

    MidiPort();
    virtual ~MidiPort() {}

    void dispatch();
    void setListener(MidiListener* pListener);

    size_t count() const { return _messageQueue.size(); }
    bool empty() const { return _messageQueue.empty(); }

protected:
    void dispatchMessage(const MidiMessage& message);
    void enqueueMessage(const MidiMessage& message);
    
    void dispatchSysEx(const std::string& sysEx);
    void enqueueSysEx(const std::string& sysEx);

private:
    MidiListener* _pListener;

    typedef std::queue<MidiMessage> messageQueue_t;
    messageQueue_t _messageQueue;

    typedef std::queue<std::string> sysExQueue_t;
    sysExQueue_t _sysExQueue;

    uint16_t _rpn_param = 0x8000;
    uint16_t _nrpn_param = 0x8000;
    uint16_t _param_value = 0;

    SemaphoreHandle_t _mutex;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_MIDIPORT_H