#ifndef ADSConverter
#define ADSConverter

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <variable.h>

class ADSInit {
public:
	Adafruit_ADS1115 ads; // use this for the 16-bit version
	bool init();
};

class SEN_0189 : public ADSInit {
public:
	void run(uint8_t pin, long delayms, int* adc_ads, float* volt_ads);
	
private:
	unsigned long LastDelay = 0;
};

class SEN_0257 : public ADSInit {
public:
	void run(uint8_t pin, long delayms, int* adc_ads, float* volt_ads);

private:
	unsigned long LastDelay = 0;
};

#endif
