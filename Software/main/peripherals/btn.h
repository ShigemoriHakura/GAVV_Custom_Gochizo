#pragma once

class Btn {
    public:
        static Btn &getInstance();
        Btn(const Btn &) = delete;
        Btn &operator=(const Btn &) = delete;
        void init();
    private:
        Btn();
        bool isInited;
};
