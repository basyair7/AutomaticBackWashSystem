#include <WebServer>

void WebServer::WiFiConfig_1(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "wificonfig1.html", "r");
    if (!file) {
        req->send_P(404, "text/plain", "File not found");
        return;
    }

    // read file content
    size_t fileSize = file.size();
    char* htmlBuffer = new char[fileSize + 1];
    file.readBytes(htmlBuffer, fileSize);
    htmlBuffer[fileSize] = '\0';
    file.close();

    // get ip address
    IPAddress clientIP = req->client()->localIP();
    LOCALIP = clientIP.toString() + ":" + String(_port);

    const char* placeholders[] = {
        "%VERSIONPROJECT%", "%VERSIONPROJECT%",
        "%APNAME%", "%APPASSWORD%", "%APNAME%"
    };

    const char* tags_html[] = {
        version_project.c_str(), version_project.c_str(),
        APNAME_Server.c_str(), APPASSWORD_Server.c_str(),
        APNAME_Server.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());

    delete[] htmlBuffer;
}

void WebServer::WiFiConfig_2(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "wificonfig2.html", "r");
    if (!file) {
        req->send_P(200, "text/plain", "File not found");
        return;
    }

    // read file content
    size_t fileSize = file.size();
    char* htmlBuffer = new char[fileSize + 1];
    file.readBytes(htmlBuffer, fileSize);
    htmlBuffer[fileSize] = '\0';
    file.close();

    // get ip address
    IPAddress clientIP = req->client()->localIP();
    LOCALIP = clientIP.toString() + ":" + String(_port);

    const char* placeholders[] = {
        "%VERSIONPROJECT%", "%VERSIONPROJECT%", 
        "%SSID%", "%PASSWORD%", "%LOCALIP%"
    };

    const char* tags_html[] = {
        version_project.c_str(), version_project.c_str(), 
        SSID_Client.c_str(), PASSWORD_Client.c_str(),
        LOCALIP.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());
    delete[] htmlBuffer;
}

void WebServer::SaveWiFiConfig(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "save-wifi-config.html", "r");
    if (!file) {
        req->send_P(404, "text/plain", "File not found");
        return;
    }

    // read file content
    size_t fileSize = file.size();
    char* htmlBuffer = new char[fileSize + 1];
    file.readBytes(htmlBuffer, fileSize);
    htmlBuffer[fileSize] = '\0';
    file.close();

    String NEWSSIDCLIENT, NEWPASSWORDCLIENT;
    if (req->hasArg("newssid") && req->hasArg("newpassword")) {
        NEWSSIDCLIENT     = req->arg("newssid");
        NEWPASSWORDCLIENT = req->arg("newpassword");
        spiffs.changeConfigWifi(NEWSSIDCLIENT, NEWPASSWORDCLIENT);
    }

    // get ip address
    IPAddress clientIP = req->client()->localIP();
    LOCALIP = clientIP.toString() + ":" + String(_port);

    const char* placeholders[] = {
        "%VERSIONPROJECT%",
        "%NEWSSID%", "%NEWPASSWORD%",
        "%LOCALIP%"
    };

    const char* tags_html[] = {
        version_project.c_str(),
        NEWSSIDCLIENT.c_str(), NEWPASSWORDCLIENT.c_str(),
        LOCALIP.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());
    // don't forget to free the memory after use
    delete[] htmlBuffer;
}

void WebServer::WiFiConfig(AsyncWebServerRequest *req) {
    if (WiFi.getMode() != WIFI_AP) 
        this->WiFiConfig_1(req);
    else
        this->WiFiConfig_2(req);
}