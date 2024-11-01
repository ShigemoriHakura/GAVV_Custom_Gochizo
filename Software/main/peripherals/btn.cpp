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
#include "../config/config.h"

static const char *TAG = "Button";

static QueueHandle_t gpio_evt_queue = NULL;
static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    auto v = gpio_get_level(PIN_G02);
    xQueueSendFromISR(gpio_evt_queue, &v, NULL);
}

void task(void *context)
{
    gpio_num_t v;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &v, portMAX_DELAY))
        {
            if(Config::getInstance().getInt("Gochizo", "Random") == 1)
            {
                gpio_uninstall_isr_service();
                ESP_LOGI(TAG, "intr happened");
                Gochizo::getInstance().roll();
                vTaskDelay(pdMS_TO_TICKS(300));
                ESP_LOGI(TAG, "reinstall intr");
                xQueueReset(gpio_evt_queue);
                ESP_ERROR_CHECK(gpio_install_isr_service(0));
                ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_G02, gpio_isr_handler, (void *)PIN_G02));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


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
    gpio_evt_queue = xQueueCreate(10, sizeof(int));


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

    gpio_config_t oa_conf = {
        .pin_bit_mask = BIT64(PIN_G02),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    ESP_ERROR_CHECK(gpio_config(&oa_conf));
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_G02, gpio_isr_handler, (void *)PIN_G02));

    xTaskCreate(task, "btn", 4096, this, 5, NULL);
}
