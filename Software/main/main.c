/* SPI Master Half Duplex EEPROM example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include <iot_button.h>

#include "sdkconfig.h"
#include "esp_log.h"

#define Res_HOST SPI2_HOST
#define PIN_NUM_MOSI 7
#define PIN_NUM_CLK 6
#define PIN_LED 8
#define PIN_Wake 0
#define PIN_RrA_CS 2
#define PIN_RrB_CS 3

static const char TAG[] = "Gavv_C3_Res";

static spi_device_handle_t s_spiHandle;
static void SetSPI(uint8_t res, int pin);
void print_wakeup_reason();
static void check_wakeup();
static void Roll_New_Gochizo(void *arg, void *usr_data);

//                       5.6k 6.8k 10k  12k  16k  20k  24k  30k  39k  47k  62k
int RrX_final_array[] = {242, 239, 230, 225, 215, 204, 195, 179, 156, 135, 97};

void app_main(void)
{
  ESP_LOGI(TAG, "HW: V2");
  ESP_LOGI(TAG, "FW: V1.0.1");
  gpio_set_direction(PIN_Wake, GPIO_MODE_INPUT);
  gpio_set_pull_mode(PIN_Wake, GPIO_PULLUP_PULLDOWN);

  // gpio_deep_sleep_hold_en();
  ESP_LOGI(TAG, "Enabling EXT0 wakeup on pin GPIO %d", PIN_Wake);
  ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_Wake), ESP_GPIO_WAKEUP_GPIO_HIGH));

  print_wakeup_reason();

  ESP_LOGI(TAG, "Initializing bus SPI%d...", Res_HOST + 1);
  spi_bus_config_t buscfg = {
      .miso_io_num = -1,
      .mosi_io_num = PIN_NUM_MOSI,
      .sclk_io_num = PIN_NUM_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
  };

  ESP_ERROR_CHECK(spi_bus_initialize(Res_HOST, &buscfg, 0));

  spi_device_interface_config_t spiDeviceConfig =
      {
          .clock_speed_hz = 5000000,
          .mode = 0,
          .spics_io_num = -1,
          .queue_size = 7,
          .flags = SPI_DEVICE_HALFDUPLEX};

  ESP_ERROR_CHECK(spi_bus_add_device(Res_HOST, &spiDeviceConfig, &s_spiHandle));

  gpio_set_direction(PIN_RrA_CS, GPIO_MODE_OUTPUT);
  gpio_set_pull_mode(PIN_RrA_CS, GPIO_FLOATING);
  gpio_set_direction(PIN_RrB_CS, GPIO_MODE_OUTPUT);
  gpio_set_pull_mode(PIN_RrB_CS, GPIO_FLOATING);

  button_config_t gpio_btn_cfg = {
      .type = BUTTON_TYPE_GPIO,
      .gpio_button_config = {
          .gpio_num = GPIO_NUM_9,
          .active_level = 0,
      },
  };
  button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
  iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, Roll_New_Gochizo, NULL);

  SetSPI(225, PIN_RrA_CS);
  //SetSPI(97, PIN_RrA_CS);
  SetSPI(195, PIN_RrB_CS);

  vTaskDelay(pdMS_TO_TICKS(60 * 3 * 1000));
  while (1)
  {
    check_wakeup();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static void Roll_New_Gochizo(void *arg, void *usr_data)
{
  ESP_LOGI(TAG, "Roll started!");
  int nRet = rand() % (sizeof(RrX_final_array) / sizeof(int));
  ESP_LOGI(TAG, "RrA using %d", RrX_final_array[nRet]);
  SetSPI(RrX_final_array[nRet], PIN_RrA_CS);

  nRet = rand() % (sizeof(RrX_final_array) / sizeof(int));
  ESP_LOGI(TAG, "RrB using %d", RrX_final_array[nRet]);
  SetSPI(RrX_final_array[nRet], PIN_RrB_CS);
  ESP_LOGI(TAG, "Roll ended!");
}

static void SetSPI(uint8_t res, int pin)
{
  esp_err_t ret;
  spi_transaction_t trans_desc = {
      .flags = SPI_TRANS_USE_TXDATA,
      .tx_data = {res},
      .length = 8,
  };

  gpio_set_level(pin, 0);
  ESP_LOGI(TAG, "Writing '0%d'", res);
  vTaskDelay(pdMS_TO_TICKS(10));

  ret = spi_device_polling_transmit(s_spiHandle, &trans_desc); // Transmit!
  assert(ret == ESP_OK);                                       // Should have had no issues.
  ESP_LOGI(TAG, "Writing done");
  gpio_set_level(pin, 1);
  vTaskDelay(pdMS_TO_TICKS(10));
}

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    ESP_LOGI(TAG, "Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    ESP_LOGI(TAG, "Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    ESP_LOGI(TAG, "Wakeup caused by ULP program");
    break;
  default:
    ESP_LOGI(TAG, "Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

static void check_wakeup()
{
  if (gpio_get_level(PIN_Wake))
  {
    return;
  }
  ESP_LOGI(TAG, "Wake low, jump to sleep");
  iot_button_stop();
  esp_deep_sleep_start();
}