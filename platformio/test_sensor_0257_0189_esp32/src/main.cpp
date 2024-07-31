#include <Arduino.h>
// #include "test_sen_0189.cpp"
#include "test_relay.cpp"
// #include "helloworld.cpp"

// TestProgram_0189 p1;
TestRelay testrelay;
// HelloWorld hello;

void setup() {
	// p1.setup();
	testrelay.setup();
	// hello.setup();
}

void loop () {
	// p1.loop();
	testrelay.loop();
	// hello.loop();
}