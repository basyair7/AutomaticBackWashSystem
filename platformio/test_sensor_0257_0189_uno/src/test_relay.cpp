#include <Arduino.h>
#include <StandardCplusplus.h>
#include <vector>

#define on LOW
#define off HIGH

class TestRelay {
public:
	void setup() {
		Serial.begin(115200);
		for (auto i : relays) {
			pinMode(i, OUTPUT);
		}
		for (auto i : relays) {
			digitalWrite(i, off);
		}
	}
	
	void loop() {
		if ((unsigned long) (millis() - lastmillis) >= 1000) {
			lastmillis = millis();
			digitalWrite(relays[currentRelay], off);
			currentRelay = (currentRelay + 1) % relays.size();
			digitalWrite(relays[currentRelay], on);
			
			Serial.print("Relay aktif : ");
			Serial.println(currentRelay + 1);
		}
	}
		
private:
	const uint8_t relay_1 = 4;
	const uint8_t relay_2 = 5;
	const uint8_t relay_3 = 6;
	const uint8_t relay_4 = 7;
	const std::vector<uint8_t> relays = {
		relay_1, relay_2, relay_3, relay_4
	};
	int currentRelay = 0;
	
	unsigned long lastmillis = 0;
	unsigned long lasttime1 = 0, lasttime2 = 0, lasttime3 = 0, lasttime4 = 0;
};