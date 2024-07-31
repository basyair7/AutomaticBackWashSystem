#include <WebServer>

void WebServer::recovery(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "recovery.html", "r");
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
        "%LOCALIP%", "%VERSIONPROJECT%", "%HWVERSION%", "%SWVERSION%",
        "%BUILDDATE%", "%FIRMWAREREGION%", "%LOCALIP%", "%MODEWIFI%",
        "%APNAME%", "%APPASSWORD%", "%SSIDWIFI%", "%PASSWORD%",
        "%LOCALIP%", "%LOCALIP%", "%LOCALIP%", "%LOCALIP%",
        "%LOCALIP%"
    };

    const char* tags_html[] = {
        // tag 1
        LOCALIP.c_str(),
        // tags 2 - 4
        version_project.c_str(), hwversion.c_str(), swversion.c_str(),
        // tags 5 - 7
        builddate.c_str(), region.c_str(), LOCALIP.c_str(),
        // tag 8
        (WifiState ? "Client WiFi" : "Access Point"),
        // tags 9 - 12
        APNAME_Server.c_str(), APPASSWORD_Server.c_str(), SSID_Client.c_str(), PASSWORD_Client.c_str(),
        // tags 13 - 17
        LOCALIP.c_str(), LOCALIP.c_str(), LOCALIP.c_str(), LOCALIP.c_str(), LOCALIP.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());
    // don't forget to free the memory after use
    delete[] htmlBuffer;
}