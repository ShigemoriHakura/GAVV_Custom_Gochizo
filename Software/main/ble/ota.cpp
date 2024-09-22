
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"

#include "esp_log.h"
#include "esp_ota_ops.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "ble_ota.h"
#include "ota.h"
#include "esp_encrypted_img.h"

#define OTA_RINGBUF_SIZE                    8192
#define OTA_TASK_SIZE                       8192

static const char *TAG = "BLE_OTA";

extern const char rsa_private_pem_start[] asm("_binary_private_pem_start");
extern const char rsa_private_pem_end[]   asm("_binary_private_pem_end");
esp_decrypt_handle_t decrypt_handle;

OTA &OTA::getInstance()
{
    static OTA instance;
    return instance;
}

OTA::OTA() : isInited(false)
{
}

void OTA::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG, "Initializing OTA");
}
