#include <WebServer>

void WebServer::Reset_System(AsyncWebServerRequest *req) {
    file = SPIFFS.open(webpath + "reset-system.html", "r");
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
        "%VERSIONPROJECT%", "%LOCALIP%"
    };

    const char* tags_html[] = {
        version_project.c_str(), LOCALIP.c_str()
    };

    String page = htmlBuffer;
    for (int i = 0; i < (int)(sizeof(tags_html)/sizeof(tags_html[0])); i++) {
        page.replace(placeholders[i], tags_html[i]);
    }

    req->send_P(200, "text/html", page.c_str());
    delete[] htmlBuffer;

    // reinitialize config
    spiffs.reinitializeConfig();
    spiffs.reinitializeState();
    // spiffs.reinitializeVarRelay();
}