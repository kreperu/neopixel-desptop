#include "ws2812b.h"

void putPixel(uint8_t r, uint8_t g, uint8_t b, WS2812* instance) {
    pio_sm_put_blocking(instance->pio, instance->sm, (((uint32_t)(r)<<8)|((uint32_t)(g)<<16)|((uint32_t)(b)))<<8u);
    return;
}
void putPixelHex(uint32_t rgb, WS2812* instance) {
    uint8_t b = rgb & 0x000000ff;
    uint8_t g = (rgb & 0x0000ff00) >> 8;
    uint8_t r = (rgb & 0x00ff0000) >> 16;
    pio_sm_put_blocking(instance->pio, instance->sm, (((uint32_t)(r)<<8)|((uint32_t)(g)<<16)|((uint32_t)(b)))<<8u);
    return;
}

void resetWait() {
    sleep_us(resetDelayUs);
    return;
}

WS2812 ws2812bInit(PIO piows, uint8_t dout) {
    uint offset0 = pio_add_program(piows, &pwm_program);
    uint sm0 = pio_claim_unused_sm(piows, 1);
    pwm_program_init(piows, sm0, offset0, dout, clock_get_hz(clk_sys) / CLK_PIO_NEOPIXEL);
    pio_sm_set_enabled(piows, sm0, 1);
    WS2812 ret;
    ret.pio = piows;
    ret.sm = sm0;
    return ret;
}