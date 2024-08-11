/*! @file tools.cpp
 * @version 1.0.0
*/

#include <fs>

String FSProgram::readconfig(String path) {
    String val = "";
    file = LittleFS.open(path, "r");
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

void FSProgram::writeconfig(String path, String valJson) {
    if (LittleFS.exists(path)) {
        LittleFS.remove(path);
    }
    
    file = LittleFS.open(path, "w");
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

bool FSProgram::removeFile(String path) {
    if (!LittleFS.exists(path)) {
        return false;
    }

    return LittleFS.remove(path);
}
