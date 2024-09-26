#include "tpl0501.h"
#include <esp_log.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"

static const char *TAG = "TPL0501";

TPL0501 &TPL0501::getInstance()
{
    static TPL0501 instance;
    return instance;
}

TPL0501::TPL0501() : isInited(false)
{
}

static spi_device_handle_t spiHandle;
void TPL0501::init(spi_host_device_t spiHost, int mosiPin, int sclkPin)
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing TPL0501");
    spi_bus_config_t buscfg = {
        .mosi_io_num = mosiPin,
        .miso_io_num = -1,
        .sclk_io_num = sclkPin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    ESP_ERROR_CHECK(spi_bus_initialize(spiHost, &buscfg, 0));

    spi_device_interface_config_t spiDeviceConfig =
        {
            .mode = 0,
            .clock_speed_hz = 5000000,
            .spics_io_num = -1,
            .flags = SPI_DEVICE_HALFDUPLEX,
            .queue_size = 7,
        };

    ESP_ERROR_CHECK(spi_bus_add_device(spiHost, &spiDeviceConfig, &spiHandle));
}

bool TPL0501::setValue(uint8_t resValue, gpio_num_t csPin)
{
    if (!isInited)
    {
        return false;
    }

    ESP_LOGI(TAG, "Set CS pin %d using %d", csPin, resValue);
    esp_err_t ret;
    spi_transaction_t trans_desc = {
        .flags = SPI_TRANS_USE_TXDATA,
        .length = 8,
        .tx_data = {resValue},
    };

    gpio_set_level(csPin, 0);
    vTaskDelay(pdMS_TO_TICKS(10));

    ret = spi_device_polling_transmit(spiHandle, &trans_desc);
    assert(ret == ESP_OK);
    gpio_set_level(csPin, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
    return true;
}
