/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "BLEMidi.h"

#include "bt.h"
#include "bta_api.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"

F_USE_NAMESPACE

static const char* TAG = "BLEMidi";

bool BLEMidi::start()
{
    esp_err_t result;

    // Initialize NVS.
    result = nvs_flash_init();
    if (result == ESP_ERR_NVS_NO_FREE_PAGES || result == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        result = nvs_flash_init();
    }
    ESP_ERROR_CHECK(result);

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    result = esp_bt_controller_init(&bt_cfg);
    if (result) {
        ESP_LOGE(TAG, "%s initialize controller failed\n", __func__);
        return false;
    }

    result = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (result) {
        ESP_LOGE(TAG, "%s enable controller failed\n", __func__);
        return false;
    }
    result = esp_bluedroid_init();
    if (result) {
        ESP_LOGE(TAG, "%s init bluetooth failed\n", __func__);
        return false;
    }
    result = esp_bluedroid_enable();
    if (result) {
        ESP_LOGE(TAG, "%s enable bluetooth failed\n", __func__);
        return false;
    }

    result = esp_ble_gatts_register_callback(gatts_event_handler);
    if (result){
        ESP_LOGE(TAG, "gatts register error, error code = %x", result);
        return false;
    }
    result = esp_ble_gap_register_callback(gap_event_handler);
    if (result){
        ESP_LOGE(TAG, "gap register error, error code = %x", result);
        return false;
    }
    result = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    if (result){
        ESP_LOGE(TAG, "gatts app register error, error code = %x", result);
        return false;
    }
    result = esp_ble_gatts_app_register(PROFILE_B_APP_ID);
    if (result){
        ESP_LOGE(TAG, "gatts app register error, error code = %x", result);
        return false;
    }
    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    if (local_mtu_ret){
        ESP_LOGE(TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
        return false;
    }

    return true;
}

void BLEMidi::stop()
{

}