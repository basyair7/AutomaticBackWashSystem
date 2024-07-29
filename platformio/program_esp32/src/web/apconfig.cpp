#include <WebServer>

void WebServer::APConfig_1(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "apconfig1.html", "r");
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
    LOCALIP = clientIP.toString();

    const char* placeholders[] = {
        "%VERSIONPROJECT%", "%VERSIONPROJECT%", "%APNAME%",
        "%APPASSWORD%", "%APNAME%"
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

void WebServer::APConfig_2(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "apconfig2.html", "r");
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
    LOCALIP = clientIP.toString();

    const char* placeholders[] = {
        "%VERSIONPROJECT%", "%VERSIONPROJECT%", "%APNAME%",
        "%APPASSWORD%", "%LOCALIP%"
    };

    const char* tags_html[] = {
        version_project.c_str(), version_project.c_str(),
        APNAME_Server.c_str(), APPASSWORD_Server.c_str(),
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

void WebServer::SaveAPConfig(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "save-ap-config.html", "r");
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

    String NEWAPNAME, NEWAPPASSWORD;
    if (req->hasArg("newap") && req->hasArg("newpassword")) {
        NEWAPNAME     = req->arg("newap");
        NEWAPPASSWORD = req->arg("newpassword");
        spiffs->changeConfigAP(NEWAPNAME, NEWAPPASSWORD);
    }

    // get ip address
    IPAddress clientIP = req->client()->localIP();
    LOCALIP = clientIP.toString();

    const char* placeholders[] = {
        "%VERSIONPROJECT%", "%NEWAPNAME%",
        "%NEWAPPASSWORD%", "%LOCALIP%"
    };

    const char* tags_html[] = {
        version_project.c_str(), NEWAPNAME.c_str(),
        NEWAPPASSWORD.c_str(), LOCALIP.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());

    // don't forget to free the memory after use
    delete[] htmlBuffer;
}

void WebServer::APConfig(AsyncWebServerRequest *req) {
    if (WiFi.getMode() != WIFI_AP)
        this->APConfig_1(req);
    else
        this->APConfig_2(req);
}