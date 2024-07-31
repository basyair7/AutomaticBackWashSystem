#include <spiffs>

void SPIFFSProgram::updateConfig(const String& cfile, std::function<void (StaticJsonDocument<500>&)> updateFunc)
{
    StaticJsonDocument<500> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";

    if (readConfig == "null") {
        // Initialize default values if file does not exists
        doc["SSID"] = SSID_DEFAULT;
        doc["PASSWORD"] = PASSWORD_DEFAULT;
        doc["APNAME"] = APNAME_DEFAULT;
        doc["APPASSWORD"] = APPASSWORD_DEFAULT;
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON update config handlers: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // call the lambda function to update the configuration
    updateFunc(doc);

    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
    TSprintln(this->readconfig(cfile));
}

void SPIFFSProgram::updateState(const String& cfile, std::function<void (StaticJsonDocument<200>&)> updateFunc)
{
    StaticJsonDocument<200> doc;
    String readConfig = this->readconfig(cfile), newConfig = "";

    if (readConfig == "null") {
        // Initialze default values if file does not exists
        doc["WIFI_MODE"] = false;
        doc["AUTO_CHANGE"] = false;
        doc["REMOTE_SERVER"] = false;
    }
    else {
        DeserializationError error = deserializeJson(doc, readConfig);
        if (error) {
            TSprintln(F("Failed to deserialize JSON update config handlers: "));
            TSprintln(error.c_str());
            return;
        }
    }

    // call the lambda function to update the configuration
    updateFunc(doc);

    serializeJson(doc, newConfig);
    this->writeconfig(cfile, newConfig);
    TSprintln(this->readconfig(cfile));
}

/* Change Config State
 * stateConfig :
 * -> WIFI_MODE
 * -> AUTO_CHANGE
 * value :
 * -> true (enable)
 * -> false (disable)
*/
void SPIFFSProgram::changeConfigState(String stateConfig, bool value) {
    TSprintf("\nUpdate :\nSTATECONFIG: %s\nVALUE: %s\n", stateConfig.c_str(), value ? "true" : "false");
    this->updateState(configFileState, [&](StaticJsonDocument<200>& doc) {
        doc[stateConfig] = value;
    });
}

void SPIFFSProgram::changeConfigAP(String NEWAPNAME, String NEWAPPASSWORD)
{
    TSprintf("\nUpdate :\nNEWAPNAME: %s\nNEWAPPASSWORD: %s\n", NEWAPNAME.c_str(), NEWAPPASSWORD.c_str());
    this->updateConfig(configFile, [&](StaticJsonDocument<500>& doc) {
        doc["APNAME"] = NEWAPNAME;
        doc["APPASSWORD"] = NEWAPPASSWORD;
    });
}

void SPIFFSProgram::changeConfigWifi(String NEWSSID, String NEWPASSWORD)
{
    TSprintf("\nUpdate :\nNEWSSID: %s\nNEWPASSWORD: %s\n", NEWSSID.c_str(), NEWPASSWORD.c_str());
    this->updateConfig(configFile, [&](StaticJsonDocument<500>& doc) {
        doc["SSID"] = NEWSSID;
        doc["PASSWORD"] = NEWPASSWORD;
    });
}