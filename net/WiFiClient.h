/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#ifndef _ESP_NATIVE_NET_WIFICLIENT_H
#define _ESP_NATIVE_NET_WIFICLIENT_H

#include "../library.h"

#include "freertos/event_groups.h"
#include "esp_event.h"

F_BEGIN_NAMESPACE

class WiFiClient
{
public:
    WiFiClient();
    virtual ~WiFiClient() {}

    bool connect(const char* pSSID, const char* pPassword, uint32_t maxRetry = 8);
    void disconnect();

    bool isConnected() const { return _isConnected; }
    uint32_t ip() const { return _ipAddress; }

private:
    enum eventFlags_t {
        Failed = 1,
        Connected = 2,
        All = 3
    };

    static void _eventHandler(void* pArg, esp_event_base_t eventBase, int32_t eventId, void* pEvent);
    void _onEvent(esp_event_base_t eventBase, int32_t eventId, void* pEvent);

    int _retryCount;
    EventGroupHandle_t _wifiEvents;
    bool _isConnected;
    uint32_t _ipAddress;
};

F_END_NAMESPACE

#endif // _ESP_NATIVE_NET_WIFICLIENT_H