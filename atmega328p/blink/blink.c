#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = (1 << PB0);
	_NOP();
	PORTB = 0x0;
	_NOP();
	_delay_ms (50);
	while (1)
	{
#define DEL 100
		/* turn all on, one-by-one */
		PORTB |= (1 << PB0);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTB &= ~(1 << PB0);
		_delay_ms (DEL);
	}
}

