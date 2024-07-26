#include <Arduino.h>
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
			
			Serial.printf("Relay aktif : %d\n", currentRelay + 1);
		}
	}
		
private:
	const uint8_t relay_1 = 14;
	const uint8_t relay_2 = 27;
	const uint8_t relay_3 = 26;
	const uint8_t relay_4 = 25;
	const std::vector<uint8_t> relays = {
		relay_1, relay_2, relay_3, relay_4
	};
	int currentRelay = 0;
	
	unsigned long lastmillis = 0;
	unsigned long lasttime1 = 0, lasttime2 = 0, lasttime3 = 0, lasttime4 = 0;
};