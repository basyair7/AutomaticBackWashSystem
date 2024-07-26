#include <Arduino.h>

#define on LOW
#define off HIGH

class TestRelay {
public:
	void setup() {
		Serial.begin(115200);
		for (const uint8_t& i : relays) {
			pinMode(i, OUTPUT);
		}
		for (const uint8_t& i : relays) {
			digitalWrite(i, off);
		}
	}
	
	void mainloop() {
		if ((unsigned long) (millis() - lastmillis) >= 1000) {
			lastmillis = millis();
			digitalWrite(relays[currentRelay], off);

      const int numRelay = sizeof(relays) / sizeof(relays[0]);
      currentRelay = (currentRelay + 1) % numRelay;
			digitalWrite(relays[currentRelay], on);
			
			Serial.print("Relay aktif : ");
			Serial.println(currentRelay + 1);
		}
	}
		
private:
	unsigned long lasttime1 = 0, lasttime2 = 0, lasttime3 = 0, lasttime4 = 0;
	unsigned long lastmillis = 0;
  
  const uint8_t relay_1 = 4;
	const uint8_t relay_2 = 5;
	const uint8_t relay_3 = 6;
	const uint8_t relay_4 = 7;
	const uint8_t relays[4] = {
		relay_1, relay_2, relay_3, relay_4
	};

	int currentRelay = 0;
};