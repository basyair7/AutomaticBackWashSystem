/*! @file WebServer.cpp
 * @version 1.0.0
*/

#include <WebServer>
#include <ElegantOTA.h>

unsigned long LastReboot = 0;
void WebServer::Reboot(AsyncWebServerRequest *req) {
    String page = "{\"status\":\"server has been restart\"}";
    req->send_P(200, "application/json", page.c_str());
    if (millis() - LastReboot >= 2000) {
        LastReboot = millis();
        ESP.restart();
    }
}

void WebServer::handleNotFound(AsyncWebServerRequest *req) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += req->url();
    message += "\nMethod: ";
    message += (req->method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += req->args();
    message += "\n";
    for (uint8_t i = 0; i < req->args(); i++) {
        message += " " + req->argName(i) + ": " + req->arg(i) + "\n";
    }

    req->send_P(404, "text/plain", message.c_str());
}

void WebServer::updateOTAloop() {
    ElegantOTA.loop();
}

void WebServer::begin() {
    TSprint(F("Port: "));
    TSprintln(_port);
    
    if (!MDNS.begin("esp32-delay")) {
        TSprintln(F("Error starting mDNS"));
        return;
    }

    TSprintln(F("mDNS started"));
    MDNS.addService("http", "tcp", 80);
    ElegantOTA.begin(&server);

    using namespace std;
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
        req->send_P(200, "text/plain", "Hello ESP32!");
    });
    server.on("/status-relay", HTTP_GET, bind(&WebServer::sendRelayStatus, this, placeholders::_1));
    server.on("/data-sensor", HTTP_GET, bind(&WebServer::sendData, this, placeholders::_1));
    server.on("/post-relay", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
        bind(
            &WebServer::postRelay, this, 
            placeholders::_1,
            placeholders::_2, 
            placeholders::_3, 
            placeholders::_4, 
            placeholders::_5
        )
    );
    // run recovery page
    server.on("/recovery", HTTP_GET, bind(&WebServer::recovery, this, placeholders::_1));
    // run server system
    server.on("/reset-system", HTTP_GET, bind(&WebServer::Reset_System, this, placeholders::_1));
    // reboot web server
    server.on("/rst-webserver", HTTP_GET, bind(&WebServer::Reboot, this, placeholders::_1));

    // run config ap
    server.on("/config-ap", HTTP_GET, bind(&WebServer::APConfig, this, placeholders::_1));
    server.on("/save-ap-config", HTTP_POST, bind(&WebServer::SaveAPConfig, this, placeholders::_1));

    // run config Wifi
    server.on("/config-wifi", HTTP_GET, bind(&WebServer::WiFiConfig, this, placeholders::_1));
    server.on("/save-wifi-config", HTTP_POST, bind(&WebServer::SaveWiFiConfig, this, placeholders::_1));
    
    // get css file and javascripts file
    // CSS
    server.serveStatic("/css/recovery.css", LittleFS, "/WEB/css/recovery.css");
    server.serveStatic("/css/apconfig.css", LittleFS, "/WEB/css/apconfig.css");
    server.serveStatic("/css/wificonfig.css", LittleFS, "/WEB/css/wificonfig.css");
    // JS
    server.serveStatic("/js/reset-reboot.js", LittleFS, "/WEB/js/reset-reboot.js");
    server.serveStatic("/js/toggleCheck.js", LittleFS, "/WEB/js/toggleCheck.js");
    server.serveStatic("/js/clock.js", LittleFS, "/WEB/js/clock.js");
    server.serveStatic("/js/sweetalert.min.js", LittleFS, "/WEB/js/sweetalert.min.js");
    
    // run server
    server.onNotFound(bind(&WebServer::handleNotFound, this, placeholders::_1));
    server.begin();
    TSprintln(F("HTTP Server Started"));
}
