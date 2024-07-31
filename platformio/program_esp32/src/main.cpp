#include <main.h>
#include <ADSConverter.h>
#include <ArduinoJson.h>
#include <spiffs>
#include <programWiFi>
#include <relaycontroller>
#include <WebServer>
#include <LedBoard>
#include <BootButton>
#include <variable.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

BootButton* bootBtn = new BootButton(BOOTBUTTON, INPUT);
LEDBoard* ledBoard = new LEDBoard;
ADSInit* ads = new ADSInit;
SEN_0257* sensor1 = new SEN_0257;
SEN_0257* sensor2 = new SEN_0257;
SEN_0189* sensor3 = new SEN_0189;
SPIFFSProgram* spiffs = new SPIFFSProgram;
RelayController* relaycontrol = new RelayController;
ProgramWiFi* programWiFi = new ProgramWiFi;
WebServer* webServer = new WebServer(3000);

TaskHandle_t taskSensor = NULL;

String dataSensor;

class ProgramMain {
public:
    void runSensor(void *pvParameter) {
        (void) pvParameter;
        if (!ads->init()) {
            TSprintln(F("Task runServer: not running"));
            vTaskDelete(taskSensor);
        }

        while (true) {
            // put your main code here, to run repeatedly:
            int adc_sensor1, adc_sensor2, adc_sensor3;
            float volt_sensor1, volt_sensor2, volt_sensor3;

            StaticJsonDocument<500> doc;
            JsonObject data1, data2, data3;

            // sensor 1 (SEN 0257)
            sensor1->run(sensor_0257_1, 1000, &adc_sensor1, &volt_sensor1);
            data1 = doc.createNestedObject("data1");
            data1["name"]       = "Water Pressure 1";
            data1["adc-value"]  = adc_sensor1;
            data1["volt-value"] = volt_sensor1;
            
            // sensor 2 (SEN 0257)
            sensor2->run(sensor_0257_2, 1000, &adc_sensor2, &volt_sensor2);
            data2 = doc.createNestedObject("data2");
            data2["name"]       = "Water Pressure 2";
            data2["adc-value"]  = adc_sensor2;
            data2["volt-value"] = volt_sensor2;

            // sensor 3 (SEN 0189)
            sensor3->run(sensor_0189, 1000, &adc_sensor3, &volt_sensor3);
            data3 = doc.createNestedObject("data3");
            data3["name"]       = "Turbidity Sensor";
            data3["adc-value"]  = adc_sensor3;
            data3["volt-value"] = volt_sensor3;

            // print in terminal
            TSprintf("\nSensor 1 (SEN 0257): \nADC: %d (Volt: %.f)", adc_sensor1, volt_sensor1);
            TSprintf("\nSensor 2 (SEN 0257): \nADC: %d (Volt: %.f)", adc_sensor2, volt_sensor2);
            TSprintf("\nSensor 3 (SEN 0189): \nADC: %d (Volt: %.f)\n", adc_sensor3, volt_sensor3);

            dataSensor = "";
            serializeJson(doc, dataSensor);
            
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    void runWebServer(void *pvParameter) {
        (void) pvParameter;

        programWiFi->setup(SSID_Client, PASSWORD_Client, APNAME_Server, APPASSWORD_Server);
        programWiFi->initWiFi(WIFI_MODE_STATE);
        webServer->begin();

        this->setupTestSendData(true);

        while (true) {
            bootBtn->WiFiMode();
            ledBoard->WiFiMode(WIFI_MODE_STATE, 1000, 250);
            webServer->updateOTAloop();
            this->testSendData();

            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

private:
    bool _testProgram;
    void setupTestSendData(bool test = false) {
        if (test) {
            // Set ADC width to 12 bits (default)
            analogReadResolution(12);

            // Read analog value from the pin and use it as a seed
            int seedValue = analogRead(34);
            randomSeed(seedValue);
            TSprintln(F("Test send data: ready"));
            _testProgram = test;
        }
    }

    void testSendData() {
        if (_testProgram) {    
            StaticJsonDocument<500> data;
            JsonObject data1, data2, data3;
            int adc_sensor1, adc_sensor2, adc_sensor3;
            float volt_sensor1, volt_sensor2, volt_sensor3;

            adc_sensor1 = random(0, 65536);
            adc_sensor2 = random(0, 65536);
            adc_sensor3 = random(0, 65536);
            volt_sensor1 = random(0, 65536);
            volt_sensor2 = random(0, 65536);
            volt_sensor3 = random(0, 65536);

            data1 = data.createNestedObject("data1");
            data1["name"]       = "Water Pressure 1";
            data1["adc-value"]  = adc_sensor1;
            data1["volt-value"] = volt_sensor1;

            data2 = data.createNestedObject("data2");
            data2["name"]       = "Water Pressure 2";
            data2["adc-value"]  = adc_sensor2;
            data2["volt-value"] = volt_sensor2;

            data3 = data.createNestedObject("data3");
            data3["name"]       = "Turbidity Sensor";
            data3["adc-value"]  = adc_sensor3;
            data3["volt-value"] = volt_sensor3;

            dataSensor = "";
            
            serializeJson(data, dataSensor);
        }
    }

};

class ThisRTOS : public ProgramMain {
public:
    static void setup() {
        // put your setup code here, to run once:
            /* List baudRate
            * 1200, 2400, 4800, 9600, 19200
            * 31250, 38400, 57600, 74880, 115200 
            */

        TSbegin(115200);
        TSprintln(F("\nInitializing..."));
        spiffs->setupFS();
        bootBtn->begin();
        ledBoard->begin(LED_BUILTIN);
        relaycontrol->begin();

        ThisRTOS* p1 = new ThisRTOS;
        xTaskCreateUniversal([](void *param) {
            static_cast<ThisRTOS*>(param)->runSensor(param);
        }, "SensorMain", 8192, p1, 1, &taskSensor, 0);

        xTaskCreateUniversal([](void *param) {
            static_cast<ThisRTOS*>(param)->runWebServer(param);
        }, "WebServer", 8192, p1, 2, NULL, 1);
    }

    static void loop() {
        // put your main code here, to run repeatedly:
        // TODO (not yet implemented)
    }
};

ThisRTOS _main;
void setup() { _main.setup(); }
void loop() { _main.loop(); }
