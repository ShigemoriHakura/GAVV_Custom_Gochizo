#pragma once
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

class MCP4661 {
    public:
        static MCP4661 &getInstance();
        MCP4661(const MCP4661 &) = delete;
        MCP4661 &operator=(const MCP4661 &) = delete;
        void init(int sdaPin, int sclPin);
        bool setValue(uint8_t resAValue, uint8_t resBValue);
        i2c_master_dev_handle_t getHandle();
    private:
        MCP4661();
        bool isInited;
        i2c_master_dev_handle_t dev_handle;
};
