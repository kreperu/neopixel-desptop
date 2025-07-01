#include "server.h"
#include "website.h"

WS2812 led;

char colorstr[] = "         'in0' :'#000000','in1' : '#000000','in2' : '#000000','in3' : '#000000','in4' : '#000000','in5' : '#000000','in6' : '#000000','in7' : '#000000','in8' : '#000000','in9' : '#000000','in10' :'#000000','in11' :'#000000','in12' :'#000000','in13' :'#000000','in14' :'#000000','in15' :'#000000','in16' :'#000000','in17' :'#000000','in18' :'#000000','in19' :'#000000','in20' :'#000000','in21' :'#000000','in22' :'#000000','in23' :'#000000','in24' :'#000000','in25' :'#000000','in26' :'#000000','in27' :'#000000','in28' :'#000000','in29' :'#000000','in30' :'#000000','in31' :'#000000','in32' :'#000000','in33' :'#000000','in34' :'#000000','in35' :'#000000','in36' :'#000000','in37' :'#000000','in38' :'#000000','in39' :'#000000','in40' :'#000000','in41' :'#000000','in42' :'#000000','in43' :'#000000','in44' :'#000000','in45' :'#000000','in46' :'#000000','in47' :'#000000','in48' :'#000000','in49' :'#000000'\0";
const char tmpval[] = "50\0";

int wifiInit() {
    //Exit on init fail
    if(cyw43_arch_init_with_country(CYW43_COUNTRY_POLAND)) {
        return 255;
    }
    cyw43_arch_enable_sta_mode();
    cyw43_arch_gpio_put(0, 1); //Trun LED on (DEBUG)
    int errcode = cyw43_arch_wifi_connect_timeout_ms(SSID, PASS, CYW43_AUTH_WPA2_AES_PSK, 10000);
    if (errcode) {
        //if connection failed - retry with different auth mode
        errcode = cyw43_arch_wifi_connect_timeout_ms(SSID, PASS, 0x01400004, 10000);
    }
    if (errcode) {
        // if connection failed - retry with SSID2
        errcode = cyw43_arch_wifi_connect_timeout_ms(SSID2, PASS2, CYW43_AUTH_OPEN, 10000);
    }
    if(errcode) {
        return errcode;
    }
    cyw43_arch_gpio_put(0, 0); // Turn LED off (DEBUG)
    return 0;
}

void writeColors(uint32_t* colors) {
    for(uint16_t i = 0; i < LED_COUNT; i++) {
        putPixelHex(colors[i], &led);
    }
    resetWait();
}

void sendOK(struct tcp_pcb* client) {
    static const char ok[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 3\r\n\r\nOk!";
    tcp_write(client, ok, 67, 0);
    tcp_output(client);
    return;
}

// for the love of god make this comprehensive
void sendHTML(struct tcp_pcb* client, char* payload, uint16_t len) {                 //VMMYY - (major) Version, Month, Year
    static const char response[] = "HTTP/1.1 200 OK\r\nServer: PICOWLED/1.0 lwip/?\r\nETag: \"20625\"\r\nContent-Length: ";
    static const char end[] = "\r\nContent-Type: text/html\r\n\r\n";
    static const uint8_t responseLen = 77;
    static const uint8_t endLen = 29;
    uint8_t responseTotLen = responseLen + endLen;
    uint8_t lenLen = len >= 10000 ? 5 : len >= 1000 ? 4 : len >= 100 ? 3 : len >= 10 ? 2 : 1;
    char charLen[6];
    sprintf(charLen, "%u", len);
    char* packet = calloc((len+responseTotLen+lenLen <= SEND_BUF) ? responseTotLen + len + lenLen + 1 : SEND_BUF, 1);
    strcat(packet, response);
    strcat(packet, charLen);
    strcat(packet, end);
    strncat(packet, payload, SEND_BUF - responseTotLen - lenLen - 1);
    /*
    uint8_t charlim = 0;
    for(uint16_t i = 0; i < SEND_BUF; i++) {
        printf("%c", packet[i]);
        if(charlim >= 100) {
            printf("%c", '\n');
            charlim = 0;
        }
        ++charlim;
    }
    */
    tcp_write(client, packet, (len + responseTotLen + lenLen <= SEND_BUF) ? len + responseTotLen + lenLen : SEND_BUF - 1, 0);
    tcp_output(client);
    free(packet);
    return;
}

/**
 * @brief Respond to a get request with payload of length len, replace each {placeholder} in payload with corresponding param.
 * 
 * @param payload payload string to send.
 * @param placeholder character that should be replaced (eg. 'f': {f} = params[0] = abc).
 * @param len length of payload string.
 * @param params params that should replace placeholders (have to be null terminated \\0).
 * @param num number of params.
 */
void getReq(struct tcp_pcb* client, char* payload, char placeholder, uint16_t len, char** params, uint16_t num) {
    uint16_t totPLen = 0;
    for(uint16_t i = 0; i < num; i++) {
        char c = params[i][0];
        uint16_t j = 1;
        for(; c != '\0'; j++) c = params[i][j];
        totPLen += j;
    }
    char** chunks = calloc(num, sizeof(char*));
    char* subPayload = calloc(len+totPLen+1, 1);
    uint16_t chunkctr = 0;
    uint16_t chunklen = 0;
    uint16_t offset = 0;
    for(uint16_t i = 0; i < len-2; i++) {
        if(payload[i] == '{' && payload[i+1] == placeholder && payload[i+2] == '}') {
            //printf("%s%u", "\n\nC!", i);
            chunks[chunkctr] = calloc(chunklen + 1, 1);
            strncpy(chunks[chunkctr], payload+offset+chunkctr*3, chunklen);
            ++chunkctr;
            i += 2;
            offset += chunklen;
            chunklen = 0;
            continue;
        } else {
            ++chunklen;
        }
    }
    //chunks[chunkctr] = calloc(chunklen + 1, 1);
    //strncpy(chunks[chunkctr], payload+offset+chunkctr*3, chunklen+chunkctr+1);

    for(uint16_t i = 0; i < num; i++) {
        strcat(subPayload, chunks[i]);
        strcat(subPayload, params[i]);
    }
    //strcat(subPayload, chunks[num]);
    //printf("%s", "\n\nNaye");
    strncat(subPayload, payload+offset+chunkctr*3, chunklen+chunkctr+1);
    //printf("%s", "\n\nYay");
    for(uint16_t i = 0; i < num; i++) free(chunks[i]);
    free(chunks);
    /*
    uint8_t charlim = 0;
    for(uint16_t i = 0; i < len+totPLen; i++) {
        printf("%c", subPayload[i]);
        if(charlim >= 100) {
            printf("%c", '\n');
            charlim = 0;
        }
        ++charlim;
    }
    */
    printf("%s", "\n\n");
    sendHTML(client, subPayload, len+totPLen);
    free(subPayload);
    return;
}

err_t sendCB(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    //printf("%s", "SendSucc\n");
    return ERR_OK;
}

err_t recvCB(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    //printf("Recv\n");
    if(p == NULL) return ERR_CLSD; // if payload non-existent - error

    cyw43_arch_lwip_check(); //check lwip locks - thread safety

    if(p->tot_len <= 0) { //if payload empty - error, free
        pbuf_free(p);
        return ERR_BUF;
    }
    //printf("%u%c", p->tot_len, '\n');
    //printf("%i%c", err, '\n');
    
    // TODO: Ensure null-termination
    unsigned char* pld = (unsigned char*)p->payload; // extract payload as text

    //unsigned char* pld = (unsigned char*)calloc(p->tot_len+1, 1);



    //uint8_t buff[1024];
    //pbuf_copy_partial(p, buff, p->tot_len, 0);
    /*
    for(uint16_t i = 0; i < p->tot_len; i++) {
        //printf("%c%c", (char)buff[i], '\n');
        printf("%c", *(pld + i));
    }
    */
    if(*pld == 'G') { // starts with 'G' -> "Get"
        char ip[4] = "";
        //strstr(pld, IP_PRIMARY);
        for(uint16_t i = 0; i < p->tot_len-3; i++) {
            if(pld[i] == '8' && pld[i+1] == '.' && pld[i+2] == '1' && pld[i+3] == '.') {
                ip[0] = pld[i+4];
                ip[1] = pld[i+5];
                ip[2] = '\0';
                break;
            }
        }
        //printf("%s%s%s", "\n\n", ip, "\n\n");
        //printf("RecvCmp\nSend\n");
        char* params[3];
        params[0] = colorstr;
        params[1] = (char*)tmpval;
        params[2] = ip;
        getReq(tpcb, (char*)html, 'f', htmlSize, params, 3);
        //printf("SendCmp\n");
    }
    if(*pld == 'P') { // starts with 'P' -> "Post"/"Put"
        uint32_t data[LED_COUNT];
        uint16_t datacnt = 0;
        bool isData = 0;
        for(uint16_t i = 2; i < p->tot_len; i++) {
            if(isData) {
                char num[7] = {"\0"};
                strncpy(num, (char*)pld+i, 6);
                for(uint8_t j = 0; j < 6; j++) {
                    colorstr[(datacnt+1)*18+j] = num[j];
                }
                //printf("%s%s", "\n\n", num);
                data[datacnt] = strtol(num, NULL, 16);
                //printf("%s%lu", "\n\n", data[datacnt]);
                i+=5;
                ++datacnt;
                continue;
            }
            if(pld[i-2] == 'f' && pld[i-1] == 't' && pld[i] == 'l') {
                isData = 1;
            }
        }
        printf("%s%lu%c%s", "\n\n", data[40], ' ', colorstr);
        writeColors(data);
        sendOK(tpcb);
        //tcp_close(tpcb);
    }
    tcp_recved(tpcb, p->tot_len);

    pbuf_free(p);
    return ERR_OK;
}

//connection accept callback
err_t acceptCB (void *arg, struct tcp_pcb *newpcb, err_t err) {
    //printf("Acc\n");
    tcp_recv(newpcb, recvCB);
    tcp_sent(newpcb, sendCB);
    //printf("AccCmp\n");
    return ERR_OK;
}

int httpInit() {
    struct tcp_pcb* sock = tcp_new_ip_type(IPADDR_TYPE_V4);
    err_t binderr = tcp_bind(sock, NULL, 80);
    //printf("%i", binderr);
    struct tcp_pcb* server = tcp_listen_with_backlog(sock, 1);
    tcp_accept(server, acceptCB);
    return binderr;
}