#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int
main (void)
{
	DDRB = _BV(PB4) | _BV(PB3) | _BV(PB1); /* set outputs */
	PORTB = 0x0;

	// disable digital input buffer on ADC input
	DIDR0 = (1 << ADC1D);

	ADMUX = (0 << REFS2) | (0 << REFS1) | (0 << REFS0) |
		(0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0) | // ADC1 (PB2)
		(1 << ADLAR); // put top 8 bits in 1 register for simpler reading (discard bottom 2 bits)

	ADCSRA = (1 << ADEN) | // enable
		(1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);

	sei ();

	_delay_ms (50);
	while (1)
	{
		ADCSRA |= (1 << ADSC); // start ADC measurement
		while (ADCSRA & (1 << ADSC)) {}

		uint8_t adcval = ADCH;

		if (adcval < 128)
		{
			PORTB &= ~_BV(PB1);
			PORTB &= ~_BV(PB3);
			PORTB &= ~_BV(PB4);
		}
		else
		{
			PORTB |= _BV(PB1);
			PORTB |= _BV(PB3);
			PORTB |= _BV(PB4);
		}
	}
}

