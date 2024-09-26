#pragma once
#include "driver/gpio.h"

class Gochizo
{
public:
    static Gochizo &getInstance();
    Gochizo(const Gochizo &) = delete;
    Gochizo &operator=(const Gochizo &) = delete;
    void init();
    void setValue(uint8_t RrA, uint8_t RrB, bool needReplug);
    void roll();
    void setGPIO(gpio_num_t gpio);
    void resetGPIO(gpio_num_t gpio);
    void unPlug();
    void plug();

private:
    Gochizo();
    bool isInited;
};
