#include "pico/stdlib.h"
#include <stdio.h>
#include <tusb.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "pico/bootrom.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "ws2812b.h"
#include "color.h"
#include "server.h"

const PIO pio = pio0;

int main() {
    sleep_ms(100);
    //set_sys_clock_khz(125000, 1);
    stdio_init_all();
    sleep_us(390);
    sleep_ms(10000);

    printf("%i", 0);
    printf("%i", wifiInit());
    printf("\n");

    led = ws2812bInit(pio, 15);
    printf("2");
    httpInit();
    printf("3");

    for(;;)sleep_ms(4294967295U);
    printf("%i", 4);
    sleep_ms(100);
    return 0;
}