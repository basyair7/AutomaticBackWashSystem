/*! @file tools.cpp
 * @version 1.0.0
*/

#include <spiffs>

String SPIFFSProgram::readconfig(String path) {
    String val = "";
    file = SPIFFS.open(path, "r");
    if (file) {
        while (file.available()) {
            val += file.readString();
        }
        file.close();
    }
    else {
        TSprintln(F("Failed to open file to writing"));
        val = "null";
    }

    return val;
}

void SPIFFSProgram::writeconfig(String path, String valJson) {
    if (SPIFFS.exists(path)) {
        SPIFFS.remove(path);
    }
    
    file = SPIFFS.open(path, "w");
    if (file) {
        file.write((const uint8_t *)valJson.c_str(), valJson.length());
        file.close();
    }
    else {
        TSprint(F("Failed to create a new file "));
        TSprintln(path);
        return;
    }
}

bool SPIFFSProgram::removeFile(String path) {
    if (!SPIFFS.exists(path)) {
        return false;
    }

    return SPIFFS.remove(path);
}
