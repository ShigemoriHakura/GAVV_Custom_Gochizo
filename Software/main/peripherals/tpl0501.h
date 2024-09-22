#pragma once
#include "driver/spi_master.h"
#include "driver/gpio.h"

class TPL0501 {
    public:
        static TPL0501 &getInstance();
        TPL0501(const TPL0501 &) = delete;
        TPL0501 &operator=(const TPL0501 &) = delete;
        void init(spi_host_device_t spiHost, int mosiPin, int sclkPin);
        bool setValue(uint8_t resValue, gpio_num_t csPin);
    private:
        TPL0501();
        bool isInited;
};
