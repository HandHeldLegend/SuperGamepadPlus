/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "hoja_includes.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 64 /*!< Data buffer length of test buffer */
#define HOJA_I2C_MSG_SIZE 32

#define I2C_SLAVE_SCL_IO 20                    /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO 19                    /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(0)            /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH) /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH) /*!< I2C slave rx buffer size */
#define ESP_SLAVE_ADDR 0x76                    /*!< ESP32 slave address, you can set any 7bit value */

typedef void (*bluetooth_input_cb_t)(i2cinput_input_s *);

bluetooth_input_cb_t _bluetooth_input_cb = NULL;

uint8_t _bluetooth_mac_address[6] = {0};

hoja_settings_s global_loaded_settings = {0};

bool _i2c_read_msg(uint8_t *buffer)
{
    uint8_t idx = 0;
    static bool d = false;
    static bool e = false;
    static bool f = false;

    while(idx<HOJA_I2C_MSG_SIZE)
    {
        i2c_slave_read_buffer(I2C_SLAVE_NUM, &buffer[idx], 1, portMAX_DELAY);
        switch(buffer[idx])
        {
            default:
            break;

            case 0xDD:
            d=true;
            break;

            case 0xEE:
            e=true;
            break;

            case 0xFF:
            if(idx==(HOJA_I2C_MSG_SIZE-1))
            {
                f=true;
                idx=HOJA_I2C_MSG_SIZE;
            }
            else
            {
                f=false;
                idx=HOJA_I2C_MSG_SIZE;
            }
            break;
        }
        idx++;
    }

    i2c_reset_rx_fifo(I2C_SLAVE_NUM);

    if(d&&e&&f)
    {
        d=false;
        e=false;
        f=false;
        idx=0;
        return true;
    }
    else
    {
        d=false;
        e=false;
        f=false;
        idx=0;
        return false;
    }
}

static esp_err_t i2c_slave_init(void)
{
    int i2c_slave_port = I2C_SLAVE_NUM;
    i2c_config_t conf_slave = {
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .mode = I2C_MODE_SLAVE,
        .slave.maximum_speed = 400 * 1000,
        .slave.addr_10bit_en = 0,
        .slave.slave_addr = ESP_SLAVE_ADDR,
    };
    esp_err_t err = i2c_param_config(i2c_slave_port, &conf_slave);
    if (err != ESP_OK)
    {
        return err;
    }
    return i2c_driver_install(i2c_slave_port, conf_slave.mode, I2C_SLAVE_RX_BUF_LEN, I2C_SLAVE_TX_BUF_LEN, 0);
}

void app_input(i2cinput_input_s *input)
{
    if (!_bluetooth_input_cb)
        return;

    _bluetooth_input_cb(input);
}

void app_main(void)
{
    const char *TAG = "app_main";
    esp_err_t ret;

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(i2c_slave_init());

    static uint8_t data[HOJA_I2C_MSG_SIZE] = {0xFF, 0xFF, 0xFF};
    static i2cinput_input_s input = {0};

    for (;;)
    {
        
        vTaskDelay(1 / portTICK_PERIOD_MS);
        // size_t d_size = i2c_slave_write_buffer(I2C_SLAVE_NUM, data, 3, 1000 / portTICK_PERIOD_MS);

        if (_i2c_read_msg(data))
        {
            switch (data[0])
            {
                default:
                    ESP_LOGI(TAG, "WRONG CODE");
                    memset(data, 0, HOJA_I2C_MSG_SIZE);
                    break;

                // Initialize bluetooth
                case I2CINPUT_ID_INIT:
                {

                    input_mode_t mode = data[1];
                    global_loaded_settings.device_mac[0] = 0x7C;
                    global_loaded_settings.device_mac[1] = 0xBB;
                    global_loaded_settings.device_mac[2] = 0x8A;

                    global_loaded_settings.device_mac[3] = 0xEA;
                    global_loaded_settings.device_mac[4] = 0x30;
                    global_loaded_settings.device_mac[5] = 0x57;

                    switch (mode)
                    {
                        default:
                            break;

                        case INPUT_MODE_SWPRO:
                            _bluetooth_input_cb = switch_bt_sendinput;
                            ESP_LOGI(TAG, "Switch BT Mode Init...");
                            memset(data, 0, HOJA_I2C_MSG_SIZE);
                            core_bt_switch_start();
                            break;

                        case INPUT_MODE_XINPUT:
                            _bluetooth_input_cb = xinput_bt_sendinput;
                            ESP_LOGI(TAG, "XInput BT Mode Init...");
                            memset(data, 0, HOJA_I2C_MSG_SIZE);
                            core_bt_xinput_start();
                            break;
                    }
                }
                break;

                case I2CINPUT_ID_INPUT:
                {
                    memcpy(&input, &data[1], 27);
                    app_input(&input);
                    memset(data, 0, HOJA_I2C_MSG_SIZE);
                }
                break;
            }
        }
    }
}
