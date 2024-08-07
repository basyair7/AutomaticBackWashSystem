/*! @file programWiFi.cpp
 * @version 1.0.0
*/

#include <programWiFi>
#include <BootButton>

bool WifiState;

void ProgramWiFi::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    TSprintln(F("\nConnection to AP Successfully"));
}

void ProgramWiFi::WiFiStationDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    if (((unsigned long) (millis() - LastMillis_1) >= (long unsigned int) 15000) && WiFi.status() != WL_CONNECTED) 
    {
        LastMillis_1 = millis();
        TSprintln(F("Disconnected from WiFi Access Point"));
        TSprint(F("WiFi lost connection, Reason "));
        TSprintln(info.wifi_sta_disconnected.reason);
        TSprintln(F("Trying to reconnect"));
        WiFi.begin(ssid_sta, password_sta);
    }
}

void ProgramWiFi::WiFiGotIP(WiFiEvent_t evet, WiFiEventInfo_t info) {
    LOCALIP = WiFi.localIP().toString().c_str();
    TSprintln(F("WiFi Connected"));
    TSprint(F("IP Address: "));
    TSprintln(LOCALIP);
}

// run program mode STA
void ProgramWiFi::modeSTA() {
    BootButton bootBtn(BOOTBUTTON, INPUT);
    bootBtn.begin();
    // setup WiFi mode STA
    WiFi.mode(WIFI_STA);
    // Disabled sleep mode
    WiFi.setSleep(false);

    // register event handlers
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) { WiFiStationConnected(event, info); }, ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) { WiFiGotIP(event, info); }, ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) { WiFiStationDisconnect(event, info); }, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    // Attempt to connect to WiFi
    WiFi.begin(ssid_sta, password_sta);
    TSprintln(F("Connecting to WiFi"));
    while (WiFi.status() != WL_CONNECTED) {
        bootBtn.WiFiMode();
        TSprint(F("."));
        delay(500);
    }
    TSprintln();

    // RSSI
    TSprint(F("RSSI: "));
    TSprintln(WiFi.RSSI());
}

// run program mode AP
void ProgramWiFi::modeAP() {
    // setup WiFi mode AP
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAPConfig(local_ip, gateway, subnet)) {
        TSprintln(F("SoftAP Config Failed"));
    }
    
    if (!WiFi.softAP(Name_AP.c_str(), Password_AP.c_str())) {
        TSprintln(F("SoftAP Setup Failed"));
    }

    // get IP Address
    LOCALIP = WiFi.softAPIP().toString().c_str();
    TSprint(F("IP Address: "));
    TSprintln(LOCALIP);
}

void ProgramWiFi::setup(const String SSID, const String PASSWORD_SSID, const String NAME_AP, const String PASSWORD_AP)
{
    ssid_sta = SSID;
    password_sta = PASSWORD_SSID;
    Name_AP = NAME_AP;
    Password_AP = PASSWORD_AP;

    gateway  = IPAddress(10, 168, 43, 1);
    local_ip = IPAddress(10, 168, 43, 1);
    subnet   = IPAddress(255, 255, 255, 0);
}

/* program initialize wifi
 * Mode STA = true
 * Mode AP = false
*/
void ProgramWiFi::initWiFi(bool WiFiMode) {
    WifiState = WiFiMode;
    if (WiFiMode) {
        modeSTA();
    }
    else {
        modeAP();
    }
}