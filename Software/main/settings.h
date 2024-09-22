#pragma once
#include "driver/spi_master.h"

#define Res_HOST       SPI2_HOST
#define PIN_NUM_MOSI   GPIO_NUM_7
#define PIN_NUM_CLK    GPIO_NUM_6
#define PIN_LED        GPIO_NUM_1
#define PIN_Wake       GPIO_NUM_0
#define PIN_G02        GPIO_NUM_4
#define PIN_RrA_CS     GPIO_NUM_2
#define PIN_RrB_CS     GPIO_NUM_3

//                            5.6k 6.8k 10k  12k  16k  20k  24k  30k  39k  47k  62k
const int RrX_final_array[] = {242, 239, 230, 225, 215, 204, 195, 179, 156, 135, 97};