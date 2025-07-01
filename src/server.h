#ifndef _SERVER_H
#define _SERVER_H

#include "pico/cyw43_arch.h"
#include "wifiopts.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "ws2812b.h"
#include "color.h"

extern WS2812 led;

int wifiInit();
int httpInit();
//void getReq(struct tcp_pcb* client, char* payload, char placeholder, uint16_t len, char** params, uint16_t num);

#endif