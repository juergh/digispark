/* -*- mode: linux-c -*- */

#include <Arduino.h>
#include <util/delay.h>

/* Digispark LED */
#define PIN_LED  PB1
#define DELAY_MS 500

void setup() {
	/* Init LED pin as output */
	DDRB |= (1 << PIN_LED);

	/* Light up LED */
	PORTB |= (1 << PIN_LED);
}

void loop() {
	PORTB ^= (1 << PIN_LED);
	_delay_ms(DELAY_MS);
}
