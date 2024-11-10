
#include <esp_mac.h>
#include <esp_log.h>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLECharacteristic.h>
#include <NimBLEAdvertising.h>
#include <NimBLE2904.h>
#include "esp_sleep.h"
#include "server.h"
#include "../settings.h"
#include "../config/config.h"
#include "../gochizo/gochizo.h"

NimBLECharacteristic *statusCharacteristic;
NimBLECharacteristic *serialCharacteristic;


static const char *TAG_BLE = "BLE";

class ServerCallbacks: public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
        printf("Client address: %s\n", connInfo.getAddress().toString().c_str());
    };

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
        printf("Client disconnected - start advertising\n");
        NimBLEDevice::startAdvertising();
    };
};

class ProvCallbacks : public NimBLECharacteristicCallbacks
{
    void onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo)
    {
        printf("%s : onRead(), value: %s\n",
               pCharacteristic->getUUID().toString().c_str(),
               pCharacteristic->getValue().c_str());
    }

    void onWrite(NimBLECharacteristic *characteristic, NimBLEConnInfo &connInfo)
    {
        std::string rxValue = characteristic->getValue();
        ESP_LOG_BUFFER_HEXDUMP(TAG_BLE, rxValue.c_str(), rxValue.length(), ESP_LOG_INFO);
        uint8_t control = rxValue.c_str()[0];
        switch (control)
        {
        case 1U:
            gpio_set_level(PIN_LED, 1);
            Gochizo::getInstance().setValue(rxValue.c_str()[1], rxValue.c_str()[2], true);
            Config::getInstance().setInt("Gochizo", "Sleep", rxValue.c_str()[3]);
            Config::getInstance().setInt("Gochizo", "Random", rxValue.c_str()[4]);
            Config::getInstance().setInt("Gochizo", "Shift", rxValue.c_str()[5]);
            ESP_LOGI(TAG_BLE, "Sleep delay: %ds", Config::getInstance().getInt("Gochizo", "Sleep"));
            ESP_LOGI(TAG_BLE, "Random: %d", Config::getInstance().getInt("Gochizo", "Random"));
            ESP_LOGI(TAG_BLE, "Shift: %d", Config::getInstance().getInt("Gochizo", "Shift"));
            vTaskDelay(pdMS_TO_TICKS(300));
            MBLEServer::getInstance().syncState();
            gpio_set_level(PIN_LED, 0);
            break;
        case 2U:
            Gochizo::getInstance().plug();
            MBLEServer::getInstance().syncState();
            break;
        case 3U:
            Gochizo::getInstance().unPlug();
            MBLEServer::getInstance().syncState();
            break;
        case 4U:
            Gochizo::getInstance().unPlug();
            esp_deep_sleep_start();
            break;
        default:
            break;
        }
    }
};

MBLEServer::MBLEServer() : isInited(false) {}

MBLEServer &MBLEServer::getInstance()
{
    static MBLEServer instance;
    return instance;
}

void MBLEServer::setState(int stat)
{
    statusCharacteristic->setValue(stat);
    statusCharacteristic->notify();
}

void MBLEServer::syncState()
{
    char* statusC = new char[20];
    sprintf(statusC, "00%02X%02X%02X%02X%02X", 
        Config::getInstance().getInt("Gochizo", "RrA"),
        Config::getInstance().getInt("Gochizo", "RrB"),
        Config::getInstance().getInt("Gochizo", "Sleep"),
        Config::getInstance().getInt("Gochizo", "Random"),
        Config::getInstance().getInt("Gochizo", "Shift")
    );
    ESP_LOGI(TAG_BLE, "Calced status: %s", statusC);
    std::string rxValue = statusC;
    statusCharacteristic->setValue(rxValue);
    statusCharacteristic->notify();
}

void MBLEServer::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG_BLE, "Init BLE Server");

    uint8_t chipid[6];
    esp_efuse_mac_get_default(chipid);
    esp_read_mac(chipid, ESP_MAC_WIFI_STA);

    char hwChipID[12];
    for (int i = 0; i < 6; i++)
    {
        sprintf(hwChipID + strlen(hwChipID), "%02x", chipid[i]);
    }
    ESP_LOGI(TAG_BLE, "Chip ID: %s", hwChipID);
     
    NimBLEDevice::init("GAVV");
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new ServerCallbacks());
    pServer->advertiseOnDisconnect(true);
    ESP_LOGI(TAG_BLE, "Init BLE Service");
    NimBLEService *pService = pServer->createService(SERVICE_UUID);
    ESP_LOGI(TAG_BLE, "Init BLE Characteristic");

    statusCharacteristic = pService->createCharacteristic(
        STATUS_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::WRITE);
    serialCharacteristic = pService->createCharacteristic(
        SERIAL_UUID,
        NIMBLE_PROPERTY::READ);
    
    statusCharacteristic->setCallbacks(new ProvCallbacks());
    statusCharacteristic->addDescriptor(new NimBLE2904());
    serialCharacteristic->setValue(hwChipID);
    ESP_LOGI(TAG_BLE, "Start BLE");
    pService->start();

    statusCharacteristic->setValue(401);

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    NimBLEDevice::startAdvertising();
}
