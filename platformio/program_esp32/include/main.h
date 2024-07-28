#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "spiffs"
#include "programWiFi"

extern SPIFFSProgram* spiffs;
extern ProgramWiFi* programWiFi;

extern String dataSensor;

#endif