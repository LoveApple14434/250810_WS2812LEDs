#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
#include <stdint.h>

#define LED_NUM       64
#define CODE_0        28
#define CODE_1        56

extern uint16_t ws2812_dma_buf[LED_NUM * 24];

void ws2812_set_color(uint16_t idx, uint8_t r, uint8_t g, uint8_t b);
void ws2812_show(void);

#endif