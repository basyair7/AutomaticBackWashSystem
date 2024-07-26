#include <Arduino.h>

#define pinsensor A0

class TestProgram_0257 {
public:
	void setup() {
		Serial.begin(115200);
	}
	
	void loop() {
		if ((unsigned long) (millis() - LastMillis) >= 1000) {
			LastMillis = millis();
			
			V = analogRead(pinsensor);
			P = (V - OffSet) * 400;
			Serial.print(F("Voltage: ")); Serial.print(V);
			Serial.println(F(" V"));
			
			Serial.print(F("Pressure: ")); Serial.print(P, 1);
			Serial.println(F(" KPa"));
			Serial.println();
		}
	}
	
private:
	unsigned long LastMillis = 0;
	const float OffSet = 0.483;
	float V, P;
};