#include "MCP4661.h"
#include <esp_log.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

static const char *TAG = "MCP4661";

namespace DeviceAddress
{
    const uint8_t A0H_A1H_A2H = 0b0101111;
    const uint8_t A0L_A1H_A2H = 0b0101110;
    const uint8_t A0H_A1L_A2H = 0b0101101;
    const uint8_t A0L_A1L_A2H = 0b0101100;
    const uint8_t A0H_A1H_A2L = 0b0101011;
    const uint8_t A0L_A1H_A2L = 0b0101010;
    const uint8_t A0H_A1L_A2L = 0b0101001;
    const uint8_t A0L_A1L_A2L = 0b0101000;
}

namespace Register
{
    const uint8_t Wiper0RAM = 0x00;
    const uint8_t Wiper1RAM = 0x01;
    const uint8_t Wiper0EEPROM = 0x02;
    const uint8_t Wiper1EEPROM = 0x03;
    const uint8_t Tcon = 0x04;
    const uint8_t Status = 0x05;
} // namespace Register

namespace Command
{
    const uint8_t Write = 0b0;
    const uint8_t Increment = 0b01;
    const uint8_t Decrement = 0b10;
    const uint8_t Read = 0b11;
} // namespace Command

namespace Error
{
    uint8_t undefinedChannel = 10;
}

const uint16_t MAX_WIPER_VAL = 0x100;

static uint8_t buildFirstByte(uint8_t address, uint8_t command, uint16_t data)
{
    return (address << 4) | (command << 2) | ((data >> 8) & 0b11);
}

int readFromRegister(uint8_t registerAddress, uint16_t* val) {
    uint8_t bytes[2];
    ESP_ERROR_CHECK(i2c_master_transmit_receive(MCP4661::getInstance().getHandle(), &registerAddress, 1, &bytes[0], sizeof(bytes), 500));
    *val = ((uint16_t)(bytes[0] & 0b1) << 8) | bytes[1];
    vTaskDelay(pdMS_TO_TICKS(20));
    return 0;
}

int writeToRegister(uint8_t registerAddress, uint16_t val) {
    uint8_t bytes[2];
    bytes[0] =  registerAddress;
    bytes[1] =  val & 0xFF;
    ESP_ERROR_CHECK(i2c_master_transmit(MCP4661::getInstance().getHandle(), bytes , sizeof(bytes), 500));
    vTaskDelay(pdMS_TO_TICKS(20));
    return 0;
}

bool isBitTrue(uint8_t byteData, uint8_t position) {
  return ((byteData >> position) & 0b1) == 1;
}

MCP4661 &MCP4661::getInstance()
{
    static MCP4661 instance;
    return instance;
}

MCP4661::MCP4661() : isInited(false)
{
}

void MCP4661::init(int sdaPin, int sclPin)
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing MCP4661");

    i2c_master_bus_config_t i2c_mst_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = (gpio_num_t)sdaPin,
        .scl_io_num = (gpio_num_t)sclPin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags{
            .enable_internal_pullup = true,
        }
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DeviceAddress::A0H_A1H_A2H,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));


    uint16_t status;
    readFromRegister(buildFirstByte(Register::Status, Command::Read, 0), &status);
    ESP_LOGW(TAG, "WP   = %d", isBitTrue(status, 0));
    ESP_LOGW(TAG, "WL0  = %d", isBitTrue(status, 1));
    ESP_LOGW(TAG, "WL1  = %d", isBitTrue(status, 2));
    ESP_LOGW(TAG, "EEWA = %d", isBitTrue(status, 3));
   
    uint16_t tcon;
    readFromRegister(buildFirstByte(Register::Tcon, Command::Read, 0), &tcon);
    ESP_LOGW(TAG, "R0B  = %d", isBitTrue(tcon, 0));
    ESP_LOGW(TAG, "R0W  = %d", isBitTrue(tcon, 1));
    ESP_LOGW(TAG, "R0A  = %d", isBitTrue(tcon, 2));
    ESP_LOGW(TAG, "R0HW = %d", isBitTrue(tcon, 3));
    ESP_LOGW(TAG, "R1B  = %d", isBitTrue(tcon, 4));
    ESP_LOGW(TAG, "R1W  = %d", isBitTrue(tcon, 5));
    ESP_LOGW(TAG, "R1A  = %d", isBitTrue(tcon, 6));
    ESP_LOGW(TAG, "R1HW = %d", isBitTrue(tcon, 7));
    ESP_LOGW(TAG, "GCEN = %d", isBitTrue(tcon >> 8, 0));

    uint16_t registerRAMA;
    readFromRegister(buildFirstByte(Register::Wiper0RAM + 0, Command::Read, 0), &registerRAMA);
    uint16_t registerEEPROMA;
    readFromRegister(buildFirstByte(Register::Wiper0EEPROM + 0, Command::Read, 0), &registerEEPROMA);
    ESP_LOGE(TAG, "A RAM: %d, EEPROM %d", registerRAMA, registerEEPROMA);

    uint16_t registerRAMB;
    readFromRegister(buildFirstByte(Register::Wiper0RAM + 1, Command::Read, 0), &registerRAMB);
    uint16_t registerEEPROMB;
    readFromRegister(buildFirstByte(Register::Wiper0EEPROM + 1, Command::Read, 0), &registerEEPROMB);
    ESP_LOGE(TAG, "B RAM: %d, EEPROM %d", registerRAMB, registerEEPROMB);

    ESP_LOGI(TAG, "Initializing MCP4661");
}

bool MCP4661::setValue(uint8_t resAValue, uint8_t resBValue)
{
    if (!isInited)
    {
        return false;
    }

    resAValue = 257 - resAValue;
    resBValue = 257 - resBValue;
    writeToRegister(buildFirstByte(Register::Wiper0RAM + 0, Command::Write, resAValue), resAValue);
    writeToRegister(buildFirstByte(Register::Wiper0RAM + 1, Command::Write, resBValue), resBValue);
    ESP_LOGI(TAG, "Set 2iper %d RAM and ROM using %d", 0, resAValue);

    writeToRegister(buildFirstByte(Register::Wiper0EEPROM + 0, Command::Write, resAValue), resAValue);
    writeToRegister(buildFirstByte(Register::Wiper0EEPROM + 1, Command::Write, resBValue), resBValue);
    ESP_LOGI(TAG, "Set 2iper %d RAM and ROM using %d", 1, resBValue);

    return true;
}

i2c_master_dev_handle_t MCP4661::getHandle()
{
    return dev_handle;
}