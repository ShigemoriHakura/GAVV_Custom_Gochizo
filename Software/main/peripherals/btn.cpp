#include "btn.h"
#include <iot_button.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>
#include "../settings.h"
#include "../gochizo/gochizo.h"

static const char *TAG = "Button";

Btn &Btn::getInstance()
{
    static Btn instance;
    return instance;
}

Btn::Btn() : isInited(false)
{
}

static void roll_New_Gochizo(void *arg, void *usr_data)
{
    ESP_LOGI(TAG, "button pressed");
    Gochizo::getInstance().roll();
}

void Btn::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing button");

    gpio_set_direction(PIN_Roll, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_Roll, GPIO_PULLUP_ONLY);
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = PIN_Roll,
            .active_level = 0,
        },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, roll_New_Gochizo, NULL);
}
