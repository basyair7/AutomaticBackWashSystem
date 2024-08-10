/*! @file sendData.cpp
 * @version 1.0.0
*/

#include <WebServer>
#include <main.h>

void WebServer::sendData(AsyncWebServerRequest *req) {
    req->send_P(200, "application/json", dataSensor.c_str());
}
