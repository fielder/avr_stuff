#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = _BV(PB0) | _BV(PB6); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

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

