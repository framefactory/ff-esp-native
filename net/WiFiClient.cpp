/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "WiFiClient.h"

#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_err.h"

#include <string>
#include <cstring>


F_USE_NAMESPACE

static const char* TAG = "WiFiClient";

WiFiClient::WiFiClient() :
    _retryCount(0),
    _wifiEvents(nullptr),
    _isConnected(false),
    _ipAddress(0)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

bool WiFiClient::connect(const char* pSSID, const char* pPassword, uint32_t maxRetry /* = 8 */)
{
    _retryCount = maxRetry;
    _wifiEvents = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiClient::_eventHandler, this));
    ESP_ERROR_CHECK(esp_event_handler_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFiClient::_eventHandler, this));

    wifi_config_t config;
    memset(&config, 0, sizeof(wifi_config_t));

    config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    config.sta.pmf_cfg.capable = true;
    config.sta.pmf_cfg.required = false;

    strncpy((char*)config.sta.ssid, pSSID, sizeof(config.sta.ssid) - 1);
    strncpy((char*)config.sta.password, pPassword, sizeof(config.sta.password) - 1);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &config));

    // start WiFi and wait for event bits to be set
    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t result = xEventGroupWaitBits(_wifiEvents, eventFlags_t::All, pdFALSE, pdFALSE, portMAX_DELAY);

    if (result & eventFlags_t::Connected) {
        //ESP_LOGI(TAG, "Connected to: %s, IP: " IPSTR, pSSID, _ipAddress);
        _isConnected = true;
    }
    else if (result & eventFlags_t::Failed) {
        ESP_LOGI(TAG, "Failed to connect to: %s", pSSID);
        ESP_ERROR_CHECK(esp_wifi_stop());
        _isConnected = false;
    }

    // cleanup
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &WiFiClient::_eventHandler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiClient::_eventHandler));
    vEventGroupDelete(_wifiEvents);

    return _isConnected;
}

void WiFiClient::disconnect()
{
    if (_isConnected) {
        ESP_ERROR_CHECK(esp_wifi_disconnect());
        ESP_ERROR_CHECK(esp_wifi_stop());
        _isConnected = false;
    }
}

void WiFiClient::_eventHandler(void* pArg, esp_event_base_t eventBase, int32_t eventId, void* pEvent)
{
    WiFiClient* pClient = static_cast<WiFiClient*>(pArg);
    pClient->_onEvent(eventBase, eventId, pEvent);
}

void WiFiClient::_onEvent(esp_event_base_t eventBase, int32_t eventId, void* pEvent)
{
    // WiFi has started, now try to connect
    if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }
    // attempt to connect failed, we got disconnected, try again
    else if (eventBase == WIFI_EVENT && eventId == WIFI_EVENT_STA_DISCONNECTED) {
        if (_retryCount > 0) {
            _retryCount--;
            ESP_LOGI(TAG, "Connection attempt failed, try again");
            esp_wifi_connect();
        }
        else {
            // no more retries, set failed flag
            xEventGroupSetBits(_wifiEvents, eventFlags_t::Failed);
        }
    }
    else if (eventBase == IP_EVENT && eventId == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* pIpEvent = static_cast<ip_event_got_ip_t*>(pEvent);
        _ipAddress = pIpEvent->ip_info.ip.addr;
        xEventGroupSetBits(_wifiEvents, eventFlags_t::Connected);
    }
}
