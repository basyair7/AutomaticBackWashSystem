#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>

// defined the sensor pins
#define pinSensor_1 	0

class TestProgram_0189 {
public:
	void setup() {
		Serial.begin(115200);
		// pinMode(pinSensor_1, INPUT);
		if (!ads.begin()) {
			Serial.println("Failed to initialize ADS.\n");
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
		}
	}
	
private:
	Adafruit_ADS1115 ads; // use this for the 16-bit version
	unsigned long LastMillis = 0;
};