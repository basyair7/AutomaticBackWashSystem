/*! @file relaycontroller.cpp
 * @version 1.0.0
*/

#include <relaycontroller>

String LableRelay;
int PINIORelay, IDRelay;
bool RelayState;

void RelayController::begin(unsigned long _delay) {
    TSprintln(F("\nInitialize Relay Pins: "));
    for (const auto& i : relayKeys) {
        fs_config->parseVarRelay(i, &IDRelay, &PINIORelay, &LableRelay, &RelayState);
        if (PINIORelay == -1) {
            TSprintf("Failed to initialize relay %s\n", LableRelay.c_str());
            continue;
        }
        TSprintf("%s (%d): (%d) (%s)\n\n", LableRelay.c_str(), IDRelay, PINIORelay, RelayState ? "ON" : "OFF");
        pinMode(PINIORelay, OUTPUT);
        digitalWrite(PINIORelay, RelayState ? RELAYON : RELAYOFF);
        
        delay(_delay);
    }
}

void RelayController::write(const String relay, const bool state, unsigned long _delay) {
    if ((unsigned long) (millis() - LastMillis_1) >= (long unsigned int) _delay) {
        LastMillis_1 = millis();
        fs_config->parseVarRelay(relay, &IDRelay, &PINIORelay, &LableRelay, nullptr);
        fs_config->changeStateRelay(relay, state);
        digitalWrite(PINIORelay, state ? RELAYON : RELAYOFF);
        TSprintln(F("\nWrite Relay"));
        TSprintln(F("Relay Action : "));
        TSprint(F("Lable\t\t: "));
        TSprintln(LableRelay);
        TSprint(F("ID\t\t: "));
        TSprintln(IDRelay);
        TSprint(F("PIN I/O Relay\t: "));
        TSprintln(PINIORelay);
        TSprintf("State\t\t: %s\n\n", state ? "ON" : "OFF");
    }
}

void RelayController::write_without_save(const String relay, const bool state, unsigned long _delay)
{
    if ((unsigned long) (millis() - LastMillis_2) >= (long unsigned int) _delay)
    {
        LastMillis_2 = millis();
        fs_config->parseVarRelay(relay, &IDRelay, &PINIORelay, &LableRelay, nullptr);
        digitalWrite(PINIORelay, state ? RELAYON : RELAYOFF);
        TSprintln(F("\nWrite Relay (Without save SPIFFS)"));
        TSprintln(F("Relay Action : "));
        TSprint(F("Lable\t\t: "));
        TSprintln(LableRelay);
        TSprint(F("ID\t\t: "));
        TSprintln(IDRelay);
        TSprint(F("PIN I/O Relay\t: "));
        TSprintln(PINIORelay);
        TSprintf("State\t\t: %s\n\n", state ? "ON" : "OFF");
    }
}