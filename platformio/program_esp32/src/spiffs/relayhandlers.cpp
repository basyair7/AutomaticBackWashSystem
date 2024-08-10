/*! @file relayhandlers.cpp
 * @version 1.0.0
*/

#include <spiffs>

void SPIFFSProgram::updateRelay(const String& cfile, std::function<void (StaticJsonDocument<500>&)> updateFunc)
{
    StaticJsonDocument<500> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";
    if (readConfig == "null") {
        // Initialize default values if file does not exist
        int id = 1;
        for (int i = 0; i < (int)(sizeof(PIN_IO_RELAY)/sizeof(PIN_IO_RELAY[0])); i++) {
            JsonObject relay = doc.createNestedObject(relayKeys[i]);
            relay["id"] = id;
            relay["pin"] = PIN_IO_RELAY[i];
            relay["name"] = VALUE_DEFAULT[i];
            relay["status"] = false; // Default status to false

            id++;
        }
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON updateRelay: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // call the lambda function to update the configuration
    updateFunc(doc);
    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
}

void SPIFFSProgram::changeVarRelay(String varName, String value) {
    updateRelay(configRelay, [&](StaticJsonDocument<500>& doc) {
        doc[varName]["name"] = value;
    });
}

void SPIFFSProgram::changeStateRelay(String varName, bool status) {
    updateRelay(configRelay, [&](StaticJsonDocument<500>& doc) {
        doc[varName]["status"] = status;
    });
}

void SPIFFSProgram::changePinRelay(String varName, uint8_t _pin) {
    updateRelay(configRelay, [&](StaticJsonDocument<500>& doc) {
        doc[varName]["pin"] = _pin;
    });
}

void SPIFFSProgram::parseVarRelay(const String& varName, int* id, int* pin, String* name, bool* status)
{
    StaticJsonDocument<500> doc;
    file = SPIFFS.open(configRelay, "r");
    if (!file) {
        TSprintf("Failed to open %s file for loading", configRelay.c_str());
        return;
    }
    
    size_t fileSize = file.size();
    if (fileSize == 0) {
        file.close();
        int id = 1;
        for (int i = 0; i < (int)(sizeof(PIN_IO_RELAY)/sizeof(PIN_IO_RELAY[0])); i++) {
            JsonObject relay = doc.createNestedObject(relayKeys[i]);
            relay["id"] = id;
            relay["pin"] = PIN_IO_RELAY[i];
            relay["name"] = VALUE_DEFAULT[i];
            relay["status"] = false; // Default status to false

            id++;
        }
        String newConfig;
        serializeJson(doc, newConfig);
        writeconfig(configRelay, newConfig);
        return;
    }

    std::unique_ptr<char[]> fileBuffer(new char[fileSize + 1]);
    file.readBytes(fileBuffer.get(), fileSize);
    fileBuffer[fileSize] = '\0'; // Null-terminate the buffer
    file.close();

    DeserializationError error = deserializeJson(doc, fileBuffer.get());
    if (error)  {
        TSprintln(F("Failed to parseVarRelay: "));
        TSprintln(error.c_str());
        return;
    }
    else {
        JsonObject relay = doc[varName];
        if (id != nullptr)
            *id = relay["id"];
        if (name != nullptr)
            *name = relay["name"].as<String>();
        if (pin != nullptr)
            *pin = relay["pin"];
        if (status != nullptr)
            *status = relay["status"];
    }
}
