#include "ADSConverter.h"

bool ADSInit::init() {
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS.\n");
        return false;
    }

    return true;
}

/* 1. Pin Sensor in ADC 16bit board
 * 2. Delay
 * 3. adc result
 * 4. voltage result
*/
void SEN_0189::run(uint8_t pin, long delayms, int* adc_ads, float* volt_ads) {
    if ((unsigned long) (millis() - LastDelay) >= (long unsigned int) delayms) {
        LastDelay = millis();
        
        int _adc_ads = ADSInit::ads.readADC_SingleEnded(pin);
        float _volt = ADSInit::ads.computeVolts(_adc_ads);

        *adc_ads = _adc_ads;
        *volt_ads = _volt;
    }
}

void SEN_0257::run(uint8_t pin, long delayms, int* adc_ads, float* volt_ads) {
    if ((unsigned long) (millis() - LastDelay) >= (long unsigned int) delayms) {
        LastDelay = millis();

        int _adc_ads = this->ads.readADC_SingleEnded(pin);
        float _volt = this->ads.computeVolts(_adc_ads);

        *adc_ads = _adc_ads;
        *volt_ads = _volt;
    }
}