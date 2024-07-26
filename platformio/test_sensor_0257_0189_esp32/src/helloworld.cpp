#include <Arduino.h>

class HelloWorld {
public:
	void setup() {
		Serial.begin(115200);
		pinMode(pinLed, OUTPUT);
	}
	
	void loop() {
		Serial.println(F("Hello, world"));
		digitalWrite(pinLed, HIGH);
		delay(1000);
		digitalWrite(pinLed, LOW);
		delay(1000);
	}
	
private:
	const int pinLed = LED_BUILTIN;
};