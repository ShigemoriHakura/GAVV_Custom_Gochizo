#pragma once

class MBLEServer
{
public:
    static MBLEServer &getInstance();
    MBLEServer(const MBLEServer &) = delete;
    MBLEServer &operator=(const MBLEServer &) = delete;
    void init();
    void setState(int stat);
    void syncState();

private:
    MBLEServer();
    bool isInited;
};
