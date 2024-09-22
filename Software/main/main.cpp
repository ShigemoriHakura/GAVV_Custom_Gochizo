#include <iot_button.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"

#include "sdkconfig.h"

#include "ble/ota.h"
#include "peripherals/btn.h"
#include "peripherals/tpl0501.h"
#include "settings.h"

static const char TAG[] = "Gavv_C3";

void print_wakeup_reason();
static void check_wakeup();

extern "C" void app_main(void)
{
  ESP_LOGI(TAG, "HW: V2.4");
  ESP_LOGI(TAG, "FW: V1.0.1");

  print_wakeup_reason();

  TPL0501::getInstance().init(Res_HOST, PIN_NUM_MOSI, PIN_NUM_CLK);
  Btn::getInstance().init();

  TPL0501::getInstance().setValue(225, PIN_RrA_CS);
  TPL0501::getInstance().setValue(195, PIN_RrB_CS);

  vTaskDelay(pdMS_TO_TICKS(60 * 3 * 1000));
  //vTaskDelay(pdMS_TO_TICKS(1 * 3 * 1000));
  while (1)
  {
    check_wakeup();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
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
  if (gpio_get_level(PIN_Wake) || gpio_get_level(PIN_G02))
  {
    return;
  }
  ESP_LOGI(TAG, "Wake low, jump to sleep");
  iot_button_stop();
  esp_deep_sleep_start();
}