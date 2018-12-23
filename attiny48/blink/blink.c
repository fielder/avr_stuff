#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = _BV(PB0); /* output */
	PORTB = 0x0; /* disable pull-ups */
	_delay_ms (50);
	while (1)
	{
#define DEL 100
		/* turn all on, one-by-one */
		PORTB |= _BV(PB0);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTB &= ~_BV(PB0);
		_delay_ms (DEL);
	}
}

