#include <Arduino.h>
#include "test_sen_0189.cpp"
#include "test_sen_0257.cpp"
#include "test_relay.cpp"

// TestProgram_0189 p1;
TestProgram_0257 p2;
TestRelay p3;

void setup() {
	// p1.setup();
	// p2.setup();
	p3.setup();
}

void loop () {
	// p1.loop();
	// p2.loop();
	p3.loop();
}