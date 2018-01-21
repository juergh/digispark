/* -*- mode: linux-c -*- */

#include <Arduino.h>
#include <DigiCDC.h>

#define PIN_LED PB1

void setup() {         
	SerialUSB.begin(); 

	/* Initialize the digital pin as an output and set it */
	DDRB |= (1 << PIN_LED);
	PORTB |= (1 << PIN_LED);
}

void loop() {
	if (SerialUSB.available()) {
		SerialUSB.write(SerialUSB.read());
	} else {
		/*
		 * If you don't call a SerialUSB function (write, print, read,
		 * available, etc) every 10ms or less then you must throw in
		 * some SerialUSB.refresh(); for the USB to keep alive - also
		 * replace your delays - ie. delay(100); - with
		 * SerialUSB.delays ie. SerialUSB.delay(100);
		 */
		SerialUSB.refresh();
	}
}
