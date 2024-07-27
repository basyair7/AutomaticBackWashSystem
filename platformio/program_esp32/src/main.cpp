#include <main.h>
#include <variable.h>
#include <programWiFi>
#include <ADSConverter.h>
#include <WebServer>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

ADSInit* ads = new ADSInit;
SEN_0257* sensor1 = new SEN_0257;
SEN_0257* sensor2 = new SEN_0257;
SEN_0189* sensor3 = new SEN_0189;
ProgramWiFi* programWiFi = new ProgramWiFi;
WebServer* webServer = new WebServer(80);

TaskHandle_t taskSensor = NULL;

class ProgramMain {
public:
    void runSensor(void *pvParameter) {
        (void) pvParameter;
        if (!ads->init()) {
            vTaskDelete(taskSensor);
        }

        while (true) {
            // put your main code here, to run repeatedly:
            int16_t adc_sensor1, adc_sensor2, adc_sensor3;
            float volt_sensor1, volt_sensor2, volt_sensor3;
            // sensor 3 (SEN 0189)
            sensor3->run(sensor_0189, 1000, &adc_sensor1, &volt_sensor1);
            TSprintf("\nSensor 3 (SEN 0189): \nADC: %d (Volt: %.f)", adc_sensor1, volt_sensor1);

            // sensor 1 (SEN 0257)
            sensor1->run(sensor_0257_1, 1000, &adc_sensor2, &volt_sensor2);
            TSprintf("\nSensor 1 (SEN 0257): \nADC: %d (Volt: %.f)", adc_sensor2, volt_sensor2);
            
            // sensor 2 (SEN 0257)
            sensor2->run(sensor_0257_2, 1000, &adc_sensor3, &volt_sensor3);
            TSprintf("\nSensor 2 (SEN 0257): \nADC: %d (Volt: %.f)\n", adc_sensor3, volt_sensor3);

            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

    void runWebServer(void *pvParameter) {
        (void) pvParameter;
        pinMode(LED_BUILTIN, OUTPUT);

        programWiFi->setup(test_ssid, test_password, test_ap_name, test_ap_password);
        programWiFi->initWiFi(true);
        webServer->begin();

        while (true) {
            digitalWrite(LED_BUILTIN, HIGH);
            vTaskDelay(pdMS_TO_TICKS(200));
            digitalWrite(LED_BUILTIN, LOW);
            vTaskDelay(pdMS_TO_TICKS(200));
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
            test_ssid = "@Wifi.com 23";
            test_password = "Hostpot_ahul7";
            test_ap_name = APNAME_DEFAULT;
            test_ap_password = APPASSWORD_DEFAULT;
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
