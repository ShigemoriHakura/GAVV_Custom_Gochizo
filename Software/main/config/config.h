#pragma once
#include <string>

class Config
{
public:
    static Config &getInstance();
    Config(const Config &) = delete;
    Config &operator=(const Config &) = delete;
    void init();
    void eraseAll();
    void setString(std::string ns, std::string key, std::string value);
    void setInt(std::string ns, std::string key, int value);
    std::string getString(std::string ns, std::string key);
    int getInt(std::string ns, std::string key);
    void erase(std::string ns, std::string key);

private:
    Config();
    bool isInited;
};
