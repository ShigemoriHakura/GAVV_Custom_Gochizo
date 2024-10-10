#include "gochizo.h"
#include <esp_log.h>
#include "../settings.h"
#include "../config/config.h"
#include "../peripherals/btn.h"
#include "../peripherals/tpl0501.h"

static const char *TAG = "Gochizo";

Gochizo &Gochizo::getInstance()
{
    static Gochizo instance;
    return instance;
}

Gochizo::Gochizo() : isInited(false)
{
}

void Gochizo::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing Gochizo");

    gpio_set_direction(PIN_RrA_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_RrA_CS, GPIO_FLOATING);
    gpio_set_direction(PIN_RrB_CS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(PIN_RrB_CS, GPIO_FLOATING);

    Gochizo::getInstance().resetGPIO(PIN_G01);
}

void Gochizo::setValue(uint8_t RrA, uint8_t RrB, bool needReplug)
{
    if (!isInited)
    {
        return;
    }
    if (needReplug)
    {
        Gochizo::getInstance().unPlug();
    }

    Config::getInstance().setInt("Gochizo", "RrA", RrA);
    Config::getInstance().setInt("Gochizo", "RrB", RrB);
    TPL0501::getInstance().setValue(RrA, PIN_RrA_CS);
    TPL0501::getInstance().setValue(RrB, PIN_RrB_CS);

    if (needReplug)
    {
        Gochizo::getInstance().plug();
    }
    return;
}

void Gochizo::roll()
{
    if (!isInited)
    {
        return;
    }
    ESP_LOGI(TAG, "Roll started!");
    int nRetA = rand() % (sizeof(RrX_final_array) / sizeof(int));
    int nRetB = rand() % (sizeof(RrX_final_array) / sizeof(int));
    ESP_LOGI(TAG, "RrA: %d, RrB: %d", RrX_final_array[nRetA], RrX_final_array[nRetB]);

    Gochizo::getInstance().setValue(RrX_final_array[nRetA], RrX_final_array[nRetB], true);
    ESP_LOGI(TAG, "Roll ended!");
}

void Gochizo::setGPIO(gpio_num_t gpio)
{
    gpio_set_direction(gpio, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(gpio, GPIO_FLOATING);
    gpio_set_level(gpio, 1);
}

void Gochizo::resetGPIO(gpio_num_t gpio)
{
    gpio_config_t cfg = {
        .pin_bit_mask = BIT64(gpio),
        .mode = GPIO_MODE_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
}

void Gochizo::plug()
{
    Gochizo::getInstance().resetGPIO(PIN_G01);
}

void Gochizo::unPlug()
{
    Gochizo::getInstance().setGPIO(PIN_G01);
}