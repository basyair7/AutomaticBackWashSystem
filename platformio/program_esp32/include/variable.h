#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

// Pin sensor in ads converter 16 bit
#define sensor_0257_1   0
#define sensor_0257_2   1
#define sensor_0189     2

// environment default
#define SSID_DEFAULT        "myhotspot"
#define PASSWORD_DEFAULT    "myhotspot1234"
#define APNAME_DEFAULT      "Device-ESP32"
#define APPASSWORD_DEFAULT  "Admin#1234"
#define APIURL_DEFAULT      "https://api.example.com"
#define APIPORT_DEFAULT     "8080"
#define XMIT_BUFFER_DEFAULT 1.512   // 1.5 kilobytes
#define RECV_BUFFER_DEFAULT 1.512   // 1.5 kilobytes

class VariableMillis {
protected:
    unsigned long LastMillis = 0;
};

#endif