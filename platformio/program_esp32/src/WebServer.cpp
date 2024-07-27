#include <WebServer>
#include <ElegantOTA.h>

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

void WebServer::begin() {
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
    server.on(
        "/post-data", 
        HTTP_POST,
        [](AsyncWebServerRequest *request){},
        NULL,
        bind(
            &WebServer::postData, this, 
            placeholders::_1,
            placeholders::_2, 
            placeholders::_3, 
            placeholders::_4, 
            placeholders::_5
        )
    );

    // run server
    server.onNotFound(bind(&WebServer::handleNotFound, this, placeholders::_1));
    server.begin();
    TSprintln(F("HTTP Server Started"));
}