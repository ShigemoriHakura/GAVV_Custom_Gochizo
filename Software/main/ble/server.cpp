
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
            Gochizo::getInstance().setValue(rxValue.c_str()[1], rxValue.c_str()[2], true);
            Config::getInstance().setInt("Gochizo", "Sleep", rxValue.c_str()[3]);
            ESP_LOGI(TAG_BLE, "Sleep delay: %ds", Config::getInstance().getInt("Gochizo", "Sleep"));
            MBLEServer::getInstance().syncState();
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
    int status = (Config::getInstance().getInt("Gochizo", "Sleep") << 24) | (Config::getInstance().getInt("Gochizo", "RrB") << 16) | Config::getInstance().getInt("Gochizo", "RrA") << 8;
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
