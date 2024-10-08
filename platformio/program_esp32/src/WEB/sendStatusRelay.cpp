/*! @file sendStatusRelay.cpp
 * @version 1.0.0
*/

#include <WebServer>
#include <relaycontroller>

void WebServer::sendRelayStatusWithFS(AsyncWebServerRequest *req) {
    StaticJsonDocument<500> doc;
    String LableRelay;
    int PINIORelay, IDRelay;
    bool RelayState;
    
    for (const auto& i : relayKeys) {
        fsprogram.parseVarRelay(i, &IDRelay, &PINIORelay, &LableRelay, &RelayState);
        if (PINIORelay == -1) {
            TSprintf("Failed to initialize relay %s\n", LableRelay.c_str());
            continue;
        }
        
        JsonObject relay = doc.createNestedObject(i);
        relay["lable"] = LableRelay;
        relay["pin"] = PINIORelay;
        relay["id"] = IDRelay;
        relay["status"] = RelayState;
        
        delay(50);
    }

    String response;
    serializeJson(doc, response);
    req->send_P(200, "application/json", response.c_str());
}

void WebServer::sendRelayStatusWithoutFS(AsyncWebServerRequest *req) {
    StaticJsonDocument<500> doc;
    String LableRelay;
    int PINIORelay, IDRelay;
    bool RelayState;

    for (const auto& i : relayKeys) {
        fsprogram.parseVarRelay(i, &IDRelay, &PINIORelay, &LableRelay, nullptr);
        if (PINIORelay == -1) {
            TSprintf("Failed to initialize relay %s\n", LableRelay.c_str());
            continue;
        }

        RelayState = !digitalRead(PINIORelay);

        JsonObject relay = doc.createNestedObject(i);
        relay["lable"] = LableRelay;
        relay["pin"] = PINIORelay;
        relay["id"] = IDRelay;
        relay["status"] = RelayState;

        delay(50);
    }

    String response;
    serializeJson(doc, response);
    req->send_P(200, "application/json", response.c_str());
}

void WebServer::sendRelayStatus(AsyncWebServerRequest *req) {
    // this->sendRelayStatusWithFS(req);
    this->sendRelayStatusWithoutFS(req);
}
