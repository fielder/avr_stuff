#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = _BV(PB3); /* output */
	DDRB = _BV(PB4); /* output */
	PORTB = 0x0; /* disable pull-ups */
	_delay_ms (50);
	while (1)
	{
#define DEL 1000
		/* turn all on, one-by-one */
		PORTB |= _BV(PB3);
		PORTB |= _BV(PB4);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTB &= ~_BV(PB3);
		PORTB &= ~_BV(PB4);
		_delay_ms (DEL);
	}
}

