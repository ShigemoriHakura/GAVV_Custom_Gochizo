#include "config.h"
#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>
#include "esp_flash.h"
#include "pl_nvs.h"

static const char *TAG_Config = "Config";

Config &Config::getInstance()
{
    static Config instance;
    return instance;
}

Config::Config()
{
}

void Config::init()
{
    if (isInited)
    {
        return;
    }
    isInited = true;
    ESP_LOGI(TAG_Config, "Init Config");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NOT_INITIALIZED || err == ESP_ERR_NVS_PART_NOT_FOUND || err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND || err == ESP_ERR_NVS_INVALID_LENGTH)
    {
        ESP_LOGI(TAG_Config, "Erasing NVS partition");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
        ESP_ERROR_CHECK(err);
    }
    ESP_ERROR_CHECK(err);
    if (Config::getInstance().getString("Gochizo", "Version") != "1.0.7")
    {
        Config::getInstance().erase("Gochizo", "RrA");
        Config::getInstance().erase("Gochizo", "RrB");
        Config::getInstance().erase("Gochizo", "Sleep");
        Config::getInstance().erase("Gochizo", "Version");
        Config::getInstance().erase("Gochizo", "Random");
        Config::getInstance().erase("Gochizo", "Shift");
        ESP_LOGI(TAG_Config, "Reloading NVS partition");
        Config::getInstance().setInt("Gochizo", "RrA", 226);
        Config::getInstance().setInt("Gochizo", "RrB", 196);
        Config::getInstance().setInt("Gochizo", "Sleep", 60);
        Config::getInstance().setInt("Gochizo", "Random", 0);
        Config::getInstance().setInt("Gochizo", "Shift", 128);
        Config::getInstance().setString("Gochizo", "Version", "1.0.7");
    }
    ESP_LOGI(TAG_Config, "Config version: %s", Config::getInstance().getString("Gochizo", "Version").c_str());
    ESP_LOGI(TAG_Config, "RrA: %d, RrB: %d", Config::getInstance().getInt("Gochizo", "RrA"), Config::getInstance().getInt("Gochizo", "RrB"));
}

void Config::eraseAll()
{
    esp_err_t err = nvs_flash_erase();
    ESP_ERROR_CHECK(err);
}

void Config::setString(std::string ns, std::string key, std::string value)
{
    PL::NvsNamespace nvs(ns, PL::NvsAccessMode::readWrite);
    nvs.Write(key, value);
    nvs.Commit();
}

std::string Config::getString(std::string ns, std::string key)
{
    PL::NvsNamespace nvs(ns, PL::NvsAccessMode::readOnly);
    std::string value;
    nvs.Read(key, value);
    return value;
}

void Config::setInt(std::string ns, std::string key, int value)
{
    PL::NvsNamespace nvs(ns, PL::NvsAccessMode::readWrite);
    nvs.Write(key, (int32_t)value);
    nvs.Commit();
}

int Config::getInt(std::string ns, std::string key)
{
    PL::NvsNamespace nvs(ns, PL::NvsAccessMode::readOnly);
    int32_t value = 0;
    nvs.Read(key, value);
    return (int)value;
}

void Config::erase(std::string ns, std::string key)
{
    PL::NvsNamespace nvs(ns, PL::NvsAccessMode::readWrite);
    nvs.Erase(key);
    nvs.Commit();
}
