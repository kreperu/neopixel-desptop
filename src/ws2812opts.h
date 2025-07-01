#ifndef _WS2812OPTS_H
#define _WS2812OPTS_H

#include "hardware/pio.h"

#define CLK_PIO_NEOPIXEL 20000000.0f // Clock frequency of the WS2812 PIO (0.05us precission - 20MHz);
#define LED_COUNT 50 // number of neopixels controlled

#define resetDelayUs 50+LED_COUNT*40



#endif