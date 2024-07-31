#ifndef VARIABLE_H
#define VARIABLE_H

#include <Arduino.h>

// define pinout component
#define BOOTBUTTON 0
#define LEDDEFAULT LED_BUILTIN

// Pin sensor in ads converter 16 bit
#define sensor_0257_1   0
#define sensor_0257_2   1
#define sensor_0189     2

// define pinout relay
#define RELAY1 14
#define RELAY2 27
#define RELAY3 26
#define RELAY4 25

// environment default
inline String VAR_SWITCH    = "relay";
#define SSID_DEFAULT        "myhotspot"
#define PASSWORD_DEFAULT    "myhotspot1234"
#define APNAME_DEFAULT      "Device-ESP32"
#define APPASSWORD_DEFAULT  "Admin#1234"

inline String VALUE_DEFAULT[4] = {
    "relay 1", "relay 2", "relay 3", "relay 4"
};

inline uint8_t PIN_IO_RELAY[4] = {
    RELAY1, RELAY2, RELAY3, RELAY4
};

inline String relayKeys[4] = {
    "relay1", "relay2", "relay3", "relay4"
};

class VariableMillis {
protected:
    unsigned long LastMillis = 0;
};

extern bool WifiState;

#endif