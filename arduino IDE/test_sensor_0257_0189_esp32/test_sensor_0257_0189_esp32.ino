#include <Arduino.h>
#include "test_relay.cpp"
#include "test_sen_0189.cpp"

TestProgram_0189 p1;
TestRelay testrelay;

void setup() {
	// p1.setup();
	testrelay.setup();
}

void loop () {
	// p1.loop();
	testrelay.loop();
}