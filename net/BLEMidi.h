/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_BLEMIDI_H
#define _ESP_NATIVE_NET_BLEMIDI_H

#include "../library.h"
#include "MidiPort.h"

F_BEGIN_NAMESPACE

class BLEMidi : public MidiPort
{
public:
    BLEMidi();
    virtual ~BLEMidi() {}

    bool start();
    void stop();

protected:
    //virtual void onConnect(BLEServer* pServer) override;
    //virtual void onDisconnect(BLEServer* pServer) override;

    //virtual void onRead(BLECharacteristic* pCharacteristic) override;
	//virtual void onWrite(BLECharacteristic* pCharacteristic) override;
	//virtual void onNotify(BLECharacteristic* pCharacteristic) override;
	//virtual void onStatus(BLECharacteristic* pCharacteristic, BLECharacteristicCallbacks::Status s, uint32_t code) override;

private:
    //BLECharacteristic* _pCharacteristic = nullptr;
    bool _deviceConnected = false;
    char _runningStatus = 0;

    char _sysex[128];
    int _sysexPtr;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_BLEMIDI_H