
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
NimBLECharacteristic *rrACharacteristic;
NimBLECharacteristic *rrBCharacteristic;
NimBLECharacteristic *actionCharacteristic;

static const char *TAG_BLE = "BLE";

class ProvCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic *characteristic, NimBLEConnInfo &connInfo)
    {
        std::string rxValue = characteristic->getValue();
        ESP_LOG_BUFFER_HEXDUMP(TAG_BLE, rxValue.c_str(), rxValue.length(), ESP_LOG_INFO);
        uint8_t control = rxValue.c_str()[0];
        switch (control)
        {
        case 1U:
            Gochizo::getInstance().setValue(rxValue.c_str()[1], rxValue.c_str()[2], true);
            break;
        case 2U:
            Gochizo::getInstance().plug();
            break;
        case 3U:
            Gochizo::getInstance().unPlug();
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
    int status = (Config::getInstance().getInt("Gochizo", "RrB") << 16) | Config::getInstance().getInt("Gochizo", "RrA") << 8;
    ESP_LOGI(TAG_BLE, "Calced status: %02X, int %d", status, status);
    statusCharacteristic->setValue(status);
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
    char buffer[16];
    sprintf(buffer, "Gochizo - ");
    for (int i = 0; i < 3; i++)
    {
        sprintf(buffer + strlen(buffer), "%02x", chipid[i]);
    }
    ESP_LOGI(TAG_BLE, "Using ID: %s", buffer);

    NimBLEDevice::init(buffer);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    pServer->advertiseOnDisconnect(true);
    ESP_LOGI(TAG_BLE, "Init BLE Service");
    NimBLEService *pService = pServer->createService(SERVICE_UUID);
    ESP_LOGI(TAG_BLE, "Init BLE Characteristic");

    statusCharacteristic = pService->createCharacteristic(
        STATUS_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::WRITE);

    statusCharacteristic->setCallbacks(new ProvCallbacks());
    statusCharacteristic->addDescriptor(new NimBLE2904());
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
