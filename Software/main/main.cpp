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

#include "config/config.h"
#include "gochizo/gochizo.h"
#include "peripherals/btn.h"
#include "peripherals/tpl0501.h"
#include "settings.h"

#ifdef BLESERVER
#include "ble/server.h"
#endif

static const char TAG[] = "Gavv_C3";

void print_wakeup_reason();
static void check_wakeup();

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "HW: V2.11");
    ESP_LOGI(TAG, "FW: V1.0.12");

    print_wakeup_reason();

    gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT_OD);
    gpio_set_pull_mode(PIN_LED, GPIO_PULLUP_ONLY);
    gpio_set_level(PIN_LED, 0);

    ESP_LOGI(TAG, "Enabling EXT0 wakeup on pin GPIO %d", PIN_Wake);
    ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_Wake), ESP_GPIO_WAKEUP_GPIO_HIGH));


    Config::getInstance().init();
    Gochizo::getInstance().init();
    TPL0501::getInstance().init(Res_HOST, PIN_NUM_MOSI, PIN_NUM_CLK);
    Btn::getInstance().init();
#ifdef BLESERVER
    MBLEServer::getInstance().init();
    MBLEServer::getInstance().syncState();
#endif
    Gochizo::getInstance().setValue(Config::getInstance().getInt("Gochizo", "RrA"), Config::getInstance().getInt("Gochizo", "RrB"), false);

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
    if (gpio_get_level(PIN_Wake))
    {
        return;
    }
    ESP_LOGI(TAG, "Wake low, jump to sleep");
    iot_button_stop();
    Gochizo::getInstance().unPlug();
    esp_deep_sleep_start();
}