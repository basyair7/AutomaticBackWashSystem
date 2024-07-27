#include <WebServer>

void WebServer::postData(AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total) 
{
    String body = String((char*)data).substring(0, len);
    TSprintf("\nReceived data: %s\n", body.c_str());

    // Memproses data JSON
    String response = "";
    StaticJsonDocument<200> jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, body);

    if (error) {
        TSprintf("deserializeJson() failed: %s\n", error.c_str());
        response = "{\"status\":\""; 
        response += error.c_str();
        response += "\"}";

        req->send_P(400, "application/json", response.c_str());
        return;
    }

    // Memperbarui status relay berdasarkan JSON payload
    for (int i = 0; i < 4; i++) {
        if (jsonDoc.containsKey(relayKeys[i])) {
            bool relayState = jsonDoc[relayKeys[i]];
            updateRelayState(i, relayState);

            jsonDoc[relayKeys[i]] = relayState;
        }
    }
    
    jsonDoc["status"] = "success";
    
    serializeJson(jsonDoc, response);

    req->send_P(200, "application/json", response.c_str());
}

void WebServer::updateRelayState(int relay, bool state) {
    if (relay >= 0 && relay < 4) {
        // digitalWrite(relayPins[relay], state ? HIGH : LOW);
        TSprintf("relay pin %d : %s\n", relayPins[relay], state ? "ON" : "OFF");
    }
}