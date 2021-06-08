/**
 * ESP/Native GPIO Library
 * Copyright 2021 Frame Factory GmbH, Ralph Wiedemeier
 * License: MIT
 */

#include "I2CPort.h"

#include "esp_err.h"
#include "driver/i2c.h"
#include "esp32/rom/ets_sys.h"


F_USE_NAMESPACE


bool I2CPort::open(int sdaPin, int sclPin, int i2cPort /* = 0 */)
{
    if (_isOpen) {
        close();
    }

    _port = i2cPort;

    i2c_port_t port = (i2c_port_t)_port;
    gpio_pullup_t pullup = _pullupsEnabled ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;

    i2c_config_t config {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = (gpio_num_t)sdaPin,
        .scl_io_num = (gpio_num_t)sclPin,
        .sda_pullup_en = pullup,
        .scl_pullup_en = pullup,
        .master = { .clk_speed = _clockSpeed }
    };

    esp_err_t err = i2c_param_config(port, &config);
    if (err != ESP_OK) {
        printf("[I2CPort::begin] configuration error: %s\n", esp_err_to_name(err));
        return false;
    }

    err = i2c_driver_install(port, I2C_MODE_MASTER, 0, 0, 0);
    if (err != ESP_OK) {
         printf("[I2CPort::begin] installation error: %s\n", esp_err_to_name(err));
         return false;
    }

    _isOpen = true;
    return true;
}

void I2CPort::close()
{
    if (_isOpen) {
        i2c_driver_delete((i2c_port_t)_port);
        _isOpen = false;
    }
}

void I2CPort::write(uint8_t address, uint8_t* pData, size_t length)
{
    i2c_port_t port = (i2c_port_t)_port;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, pData, length, true);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(port, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);

    if (err != ESP_OK) {
        printf("[I2CPort::write] I2C transmission error: %s\n", esp_err_to_name(err));
        return;
    }

    ets_delay_us(50);
}

void I2CPort::setClockSpeed(uint32_t speed)
{
    _clockSpeed = speed;
}

void I2CPort::setPullupsEnabled(bool enabled)
{
    _pullupsEnabled = enabled;
}