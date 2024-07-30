#include <main.h>
#include <ArduinoJson.h>
#include <variable.h>
#include <BootButton>
#include <spiffs>
#include <relaycontroller>
#include <programWiFi>
#include <ADSConverter.h>
#include <WebServer>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

BootButton* bootBtn = new BootButton(BOOTBUTTON, INPUT);
ADSInit* ads = new ADSInit;
SEN_0257* sensor1 = new SEN_0257;
SEN_0257* sensor2 = new SEN_0257;
SEN_0189* sensor3 = new SEN_0189;
SPIFFSProgram* spiffs = new SPIFFSProgram;
RelayController* relaycontrol = new RelayController;
ProgramWiFi* programWiFi = new ProgramWiFi;
WebServer* webServer = new WebServer(8080);

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
        pinMode(LEDDEFAULT, OUTPUT);

        programWiFi->setup(test_ssid, test_password, test_ap_name, test_ap_password);
        programWiFi->initWiFi(AP_MODE_STATE);
        webServer->begin();

        // Set ADC width to 12 bits (default)
        analogReadResolution(12);

        // Read analog value from the pin and use it as a seed
        int seedValue = analogRead(34);
        randomSeed(seedValue);

        while (true) {
            bootBtn->WiFiMode();
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
            data1["name"]       = "SEN0257-1";
            data1["adc-value"]  = adc_sensor1;
            data1["volt-value"] = volt_sensor1;

            data2 = data.createNestedObject("data2");
            data2["name"]       = "SEN0257-2";
            data2["adc-value"]  = adc_sensor2;
            data2["volt-value"] = volt_sensor2;

            data3 = data.createNestedObject("data3");
            data3["name"]       = "SEN0189";
            data3["adc-value"]  = adc_sensor3;
            data3["volt-value"] = volt_sensor3;

            dataSensor = "";
            
            serializeJson(data, dataSensor);
            
            digitalWrite(LEDDEFAULT, HIGH);
            vTaskDelay(pdMS_TO_TICKS(200));
            digitalWrite(LEDDEFAULT, LOW);
            vTaskDelay(pdMS_TO_TICKS(200));
            
            webServer->updateOTAloop();
        }
    }

protected:
    static String test_ssid;
    static String test_password;
    static String test_ap_name;
    static String test_ap_password;
    
    static void testProgram(bool x) {
        if (x) {
            TSprintln(F("Test program enable..."));
            test_ssid        = "@Wifi.com 23";
            test_password    = "Hostpot_ahul7";
            test_ap_name     = APNAME_DEFAULT;
            test_ap_password = APPASSWORD_DEFAULT;
        }
        else {
            test_ssid        = SSID_Client;
            test_password    = PASSWORD_Client;
            test_ap_name     = APNAME_Server;
            test_ap_password = APPASSWORD_Server;
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
        relaycontrol->begin();
        ThisRTOS::testProgram(true);

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

String ProgramMain::test_ssid;
String ProgramMain::test_password;
String ProgramMain::test_ap_name;
String ProgramMain::test_ap_password;

ThisRTOS _main;
void setup() { _main.setup(); }
void loop() { _main.loop(); }
