/* -*- mode: linux-c -*- */

#include <Arduino.h>
#include <DigiCDC.h>

#define LED_DELAY  100
#define ADC_DELAY  900

#define PIN_LED PB1

void init_led()
{
	/* Initialize the LED pin as an output */
	DDRB |= (1 << PIN_LED);
}
	  
void init_adc()
{
	/* ADMUX - ADC Multiplexer Selection Register */
	ADMUX = 0;
	ADMUX |= (1 << REFS1);   /* Internal 1.1V Voltage Reference */
	ADMUX |= (1 << MUX3);    /* ADC4 */
	ADMUX |= (1 << MUX2);    /* ADC4 */
	ADMUX |= (1 << MUX1);    /* ADC4 */
	ADMUX |= (1 << MUX0);    /* ADC4 */

	/* ADCSRA - ADC Control and Status Register A */
	ADCSRA = 0;
	ADCSRA |= (1 << ADEN);    /* Enable the ADC */
	ADCSRA |= (1 << ADPS2);   /* ADC Prescaler - 16 (16MHz -> 1MHz) */
}

void do_adc()
{
	/* Turn on the LED */
	PORTB |= (1 << PIN_LED);

	/* Start the conversion */
	ADCSRA |= (1 << ADSC);

	/* Wait a bit and turn off the LED */
	SerialUSB.delay(LED_DELAY);
	PORTB &= ~(1 << PIN_LED);

	/* Wait for the ADC */
	SerialUSB.delay(ADC_DELAY);
}

void write_usb_int(int val)
{
	uint8_t buf[5];
	int i = 0;
  
	if (val < 0) {
		/* Write '-' to USB */
		SerialUSB.write(45);
		val = -val;
	}

	while (val > 0) {
		buf[i++] = (uint8_t)(val % 10);
		val /= 10;
	}

	while (i > 0) {
		/* Write ASCII code of digit to USB */
		SerialUSB.write(buf[--i] + 48);
	}
}

void setup()
{         
	SerialUSB.begin();

	init_led();
	init_adc();
}

void loop()
{
	byte lo = ADCL;
	byte hi = ADCH;

	/* Do the temperature ADC conversion */
	do_adc();

	/* Write the temperature to USB */
	write_usb_int((int)((hi << 8) | lo));
	SerialUSB.write(10);   /* Write '\n' */
}
