/**
 * ESP/Native Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "AudioOut.h"


F_USE_NAMESPACE

static const char* TAG = "AudioOut";

AudioOut::AudioOut(int bckPin, int wsPin, int dataPin)
{
    _pinConfig.bck_io_num = bckPin;
    _pinConfig.ws_io_num = wsPin;
    _pinConfig.data_in_num = I2S_PIN_NO_CHANGE;
    _pinConfig.data_out_num = dataPin;
}

bool AudioOut::open()
{
    if (_isOpen) {
        return true;
    }

    i2s_config_t config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 256,
        .use_apll = true,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0
    };

    esp_err_t result = i2s_driver_install(_port, &config, 0, NULL);

    if (result == ESP_OK) {
        i2s_set_pin(_port, &_pinConfig);
        _isOpen = true;
    }
    else {
        ESP_LOGE(TAG, "failed to install I2S driver");
        _isOpen = false;
    }

    return _isOpen;
}

void AudioOut::close()
{
    if (_isOpen) {
        _isOpen = false;
        i2s_driver_uninstall(_port);
    }
}

bool AudioOut::write(const int16_t* pData, size_t numBytes)
{
    if (!_isOpen) {
        return false;
    }

    size_t bytesWritten = 0;
    esp_err_t result = i2s_write(_port, pData, numBytes, &bytesWritten, portMAX_DELAY);
    return result == ESP_OK;
}