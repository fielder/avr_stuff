#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRC = _BV(PC7); /* output */
	PORTC = 0x0; /* disable pull-ups */
	_delay_ms (50);
	while (1)
	{
#define DEL 50
		/* turn all on, one-by-one */
		PORTC |= _BV(PC7);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTC &= ~_BV(PC7);
		_delay_ms (DEL);
	}
}

