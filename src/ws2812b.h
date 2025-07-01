#ifndef _WS2812B_H
#define _WS2812B_H

#include "pico/stdlib.h"
#include <stdint.h>
#include "hardware/pio.h"
#include "pwm.pio.h"
#include "hardware/clocks.h"
#include "ws2812opts.h"

struct ws2812 {
    PIO pio;
    uint sm;
};
typedef struct ws2812 WS2812;

void putPixel(uint8_t r, uint8_t g, uint8_t b, WS2812* instance);
void putPixelHex(uint32_t rgb, WS2812* instance);
void resetWait();
WS2812 ws2812bInit(PIO piows, uint8_t dout);

#endif