#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = 0xff; /* all outputs */
	PORTB = 0x0;

	DDRD = 0xff; /* all outputs */
	PORTD = 0x0;

	DDRD &= ~_BV(PD7); /* make pin an input */
	PORTD |= _BV(PD7); /* enable pull-up */

	_delay_ms (50);

	while (1)
	{
		uint8_t pressed = (PIND & _BV(PD7)) == 0x0;
		if (pressed)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}

