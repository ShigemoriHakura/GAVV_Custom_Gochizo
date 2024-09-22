#pragma once

class OTA {
    public:
        static OTA &getInstance();
        OTA(const OTA &) = delete;
        OTA &operator=(const OTA &) = delete;
        void init();
    private:
        OTA();
        bool isInited;
};
