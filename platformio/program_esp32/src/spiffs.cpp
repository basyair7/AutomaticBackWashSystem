/*! @file spiffs.cpp
 * @version 1.0.0
*/

#include <spiffs>

String SSID_Client = SSID_DEFAULT;
String PASSWORD_Client = PASSWORD_DEFAULT;
String APNAME_Server = APNAME_DEFAULT;
String APPASSWORD_Server = APPASSWORD_DEFAULT;

bool WIFI_MODE_STATE        = true;
bool AUTO_CHANGE_MODE       = false;
bool REMOTE_SERVER_STATE    = false;

std::vector<String> SPIFFSProgram::queryDirFS(const String path) {
    std::vector<String> files;
    File dir = SPIFFS.open(path);
    if (!dir || !dir.isDirectory()) {
        TSprintln(F("Failed to open directory"));
        return files;
    }

    File file = dir.openNextFile();
    while (file) {
        String fileInfo = "- ";
        fileInfo += file.name();
        fileInfo += " (";
        fileInfo += file.size();
        fileInfo += ")";
        files.push_back(fileInfo);
        file = dir.openNextFile();
    }

    return files;
}

void SPIFFSProgram::listFiles() {
    TSprintln(F("Listing file stored in SPIFFS : "));
    TSprintln(F("List Directory /CONFIG "));
    this->TSprintlnList(this->queryDirFS("/CONFIG"));
    TSprintln();

    TSprintln(F("List Directory /WEB/html "));
    this->TSprintlnList(this->queryDirFS("/WEB/html"));
    TSprintln();

    TSprintln(F("List Directory /WEB/css "));
    this->TSprintlnList(this->queryDirFS("/WEB/css"));
    TSprintln();
    
    TSprintln(F("List Directory /WEB/js "));
    this->TSprintlnList(this->queryDirFS("/WEB/js"));
    TSprintln();
}

void SPIFFSProgram::initializeOrUpdateConfig(const String& cfile, std::function<void(StaticJsonDocument<500>&)> updateFunc)
{
    StaticJsonDocument<500> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";
    if (readConfig == "null" || !SPIFFS.exists(cfile)) {
        // Initialize default values if file does not exists
        doc["SSID"] = SSID_DEFAULT;
        doc["PASSWORD"] = PASSWORD_DEFAULT;
        doc["APNAME"] = APNAME_DEFAULT;
        doc["APPASSWORD"] = APPASSWORD_DEFAULT;
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON initializeOrUpdateConfig: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // Call the lambda function to update the configuration
    updateFunc(doc);

    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
}

void SPIFFSProgram::initializeOrUpdateVarRelay(const String& cfile, std::function<void(StaticJsonDocument<500>&)> updateFunc)
{
    StaticJsonDocument<500> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";
    if (readConfig == "null" || !SPIFFS.exists(cfile)) {
        // Initialize default values if file does not exists
        int id = 1;
        for (int i = 0; i < (int)(sizeof(PIN_IO_RELAY)/sizeof(PIN_IO_RELAY[0])); i++) {
            JsonObject relay = doc.createNestedObject(relayKeys[i]);
            relay["id"] = id;
            relay["name"] = VALUE_DEFAULT[i];
            relay["pin"] = PIN_IO_RELAY[i];
            relay["status"] = false;

            id++;
        }
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON initializeOrUpdateVarRelay: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // call the lambda function to update the configuration
    updateFunc(doc);
    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
}

void SPIFFSProgram::initializeOrUpdateState(const String& cfile, std::function<void(StaticJsonDocument<200>&)> updateFunc)
{
    StaticJsonDocument<200> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";
    if (readConfig == "null" || !SPIFFS.exists(cfile)) {
        // Initialize default value if file does not found
        doc["WIFI_MODE"] = true;
        doc["AUTO_CHANGE"] = false;
        doc["REMOTE_SERVER"] = false;
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON initializeOrUpdateState: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // call the lambda function to update the configuration
    updateFunc(doc);

    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
}

void SPIFFSProgram::initializeConfig() {
    this->initializeOrUpdateConfig(configFile, [&](StaticJsonDocument<500>& doc) {
        SSID_Client         = doc["SSID"].as<String>();
        PASSWORD_Client     = doc["PASSWORD"].as<String>();
        APNAME_Server       = doc["APNAME"].as<String>();
        APPASSWORD_Server   = doc["APPASSWORD"].as<String>();
    });
}

void SPIFFSProgram::reinitializeConfig() {
    this->initializeOrUpdateConfig(configFile, [&](StaticJsonDocument<500>& doc) {
        doc["SSID"]         = SSID_DEFAULT;
        doc["PASSWORD"]     = PASSWORD_DEFAULT;
        doc["APNAME"]       = APNAME_DEFAULT;
        doc["APPASSWORD"]   = APPASSWORD_DEFAULT;

        SSID_Client         = SSID_DEFAULT;
        PASSWORD_Client     = PASSWORD_DEFAULT;
        APNAME_Server       = APNAME_DEFAULT;
        APPASSWORD_Server   = APPASSWORD_DEFAULT; 
    });
}

void SPIFFSProgram::initializeState() {
    initializeOrUpdateState(configFileState, [&](StaticJsonDocument<200>& data) {
        WIFI_MODE_STATE  = data["WIFI_MODE"];
        AUTO_CHANGE_MODE = data["AUTO_CHANGE"];
    });
}

void SPIFFSProgram::reinitializeState() {
    initializeOrUpdateState(configFileState, [&](StaticJsonDocument<200>& data) {
        data["WIFI_MODE"]       = true;
        data["AUTO_CHANGE"]     = false;
        data["REMOTE_SERVER"]   = false;

        WIFI_MODE_STATE     = true;
        AUTO_CHANGE_MODE    = false;
        REMOTE_SERVER_STATE = false;
    });
}

void SPIFFSProgram::initializeVarRelay() {
    this->initializeOrUpdateVarRelay(configRelay, [&](StaticJsonDocument<500>& doc) {
        
    });
}

void SPIFFSProgram::reinitializeVarRelay() {
    this->initializeOrUpdateVarRelay(configRelay, [&](StaticJsonDocument<500>& doc) {
        // initialize default values
        int id = 1;
        for (int i = 0; i < (int)(sizeof(PIN_IO_RELAY)/sizeof(PIN_IO_RELAY[0])); i++) {
            JsonObject relay = doc.createNestedObject(relayKeys[i]);
            relay["id"] = id;
            relay["name"] = VALUE_DEFAULT[i];
            relay["pin"] = PIN_IO_RELAY[i];
            relay["status"] = false; // Default status to false

            id++;
        }

        TSprintln(F("Reinitialize Variable Relay: Done"));
    });
}

void SPIFFSProgram::setupFS() {
    while (true) {
        if (!SPIFFS.begin(true)) {
            TSprintln(F("Failed..."));
            TSprintln(F("Error initializing SPIFFS, please try again..."));
            TSprintln(F("Done: Error 0x1"));
            delay(150);
        }
        else {
            this->initializeConfig();
            this->initializeState();
            this->initializeVarRelay();
            this->listFiles();
            TSprintln(F("\nConfigurate WiFi Client : "));
            TSprint(F("SSID: ")); TSprintln(SSID_Client);
            TSprint(F("PASSWORD: ")); TSprintln(PASSWORD_Client);
            TSprintln(F("\nAccess Poin Server: "));
            TSprint(F("AP Name: ")); TSprintln(APNAME_Server);
            TSprint(F("AP Password: ")); TSprintln(APPASSWORD_Server);
            delay(1000);
            TSprintln(F("Done: No Error 0x0\n"));
            break;
        }
    }
}
