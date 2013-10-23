#include <Arduino.h>
#include <main.cpp>

#include "Plotter.h"
#include "Command.h"

void setup() {
	// all pins (apart from the serian inout) set as output
	for(unsigned a=2;a<=13;a++)
		pinMode(a, OUTPUT);

	// initialise serial
	Serial.begin(9600);
}

unsigned bufferPtr = 0;
char buffer[64];

Plotter plotter(
	(unsigned char[4]){12,13,10,11},
	(unsigned char[4]){9,8,7,6},
	(unsigned char[4]){5,4,3,2});


void loop() {
	while(!Serial.available())
		;

	Command c;
	Serial >> c;

	if(c == "PA") {
		plotter.moveTo(c[0], c[1]);
		Serial.println("OK");
	}
	else if(c == "PU") {
		plotter.penUp();
		Serial.println("OK");
	}
	else if(c == "PD") {
		plotter.penDown();
		Serial.println("OK");
	}
	else {
		Serial.print("Unknown command ");
		Serial << c;
		Serial.println();
	}
}
