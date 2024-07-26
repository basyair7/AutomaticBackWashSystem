#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

#define pinSensor_1 1

class TestProgram_0257 {
public:
	void setup() {
		Serial.begin(115200);
		if (!ads.begin()) {
			Serial.println(F("Failed to initialize ADS.\n"));
			while(1);
		}
	}
	
	void loop() {
		if ((unsigned long) (millis() - LastMillis) >= 1000) {
			LastMillis = millis();
			
			int16_t adc_ads = ads.readADC_SingleEnded(pinSensor_1);
			float volt = ads.computeVolts(adc_ads);
			
			Serial.print("ADC ADS : "); Serial.println(adc_ads);
			Serial.print("ADC Volt : "); Serial.println(volt);
			
			// V = analogRead(pinsensor);
			// P = (V - OffSet) * 400;
			// Serial.print(F("Voltage: ")); Serial.print(V);
			// Serial.println(F(" V"));
			
			// Serial.print(F("Pressure: ")); Serial.print(P, 1);
			// Serial.println(F(" KPa"));
			// Serial.println();
		}
	}
	
private:
	Adafruit_ADS1115 ads; // use this for the 16-bit version
	unsigned long LastMillis = 0;
	const float OffSet = 0.483;
	float V, P;
};