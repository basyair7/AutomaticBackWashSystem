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
    bool state;
    if (!SPIFFS.exists(path)) {
        state = false;
    }

    if (SPIFFS.remove(path)) {
        state = true;
    }
    else {
        state = false;
    }
    
    return state;
}
