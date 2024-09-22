#include "btn.h"
#include <iot_button.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>
#include "tpl0501.h"
#include "../settings.h"

static const char *TAG = "Button";

Btn &Btn::getInstance()
{
    static Btn instance;
    return instance;
}

Btn::Btn() : isInited(false)
{
}

static void Roll_New_Gochizo(void *arg, void *usr_data)
{
  ESP_LOGI(TAG, "Roll started!");
  int nRet = rand() % (sizeof(RrX_final_array) / sizeof(int));
  ESP_LOGI(TAG, "RrA using %d", RrX_final_array[nRet]);
  TPL0501::getInstance().setValue(RrX_final_array[nRet], PIN_RrA_CS);

  nRet = rand() % (sizeof(RrX_final_array) / sizeof(int));
  ESP_LOGI(TAG, "RrB using %d", RrX_final_array[nRet]);
  TPL0501::getInstance().setValue(RrX_final_array[nRet], PIN_RrB_CS);
  ESP_LOGI(TAG, "Roll ended!");
}

void Btn::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing button");
    
    gpio_set_direction(PIN_Wake, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_Wake, GPIO_PULLUP_PULLDOWN);
    gpio_set_direction(PIN_G02, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_G02, GPIO_FLOATING);

    gpio_set_direction(PIN_RrA_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_RrA_CS, GPIO_FLOATING);
    gpio_set_direction(PIN_RrB_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_RrB_CS, GPIO_FLOATING);

    ESP_LOGI(TAG, "Enabling EXT0 wakeup on pin GPIO %d", PIN_Wake);
    ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_Wake), ESP_GPIO_WAKEUP_GPIO_HIGH));
    ESP_LOGI(TAG, "Enabling EXT0 wakeup on pin GPIO %d", PIN_G02);
    ESP_ERROR_CHECK(esp_deep_sleep_enable_gpio_wakeup(BIT(PIN_G02), ESP_GPIO_WAKEUP_GPIO_HIGH));

    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = GPIO_NUM_9,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, Roll_New_Gochizo, NULL);
}
