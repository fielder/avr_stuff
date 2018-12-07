#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

#include "avr_twi.h"


int
main (void)
{
	DDRB = _BV(PB6) | _BV(PB7) | _BV(PB0);
	DDRD = _BV(PD5) | _BV(PD6) | _BV(PD7);
	_NOP();
	PORTB = 0x0;
	PORTD = 0x0;
	_NOP();
	_delay_ms (50);
	while (1)
	{
#define DEL 50
		/* turn all on, one-by-one */
		PORTB |= _BV(PB0);
		_delay_ms (DEL);
		PORTD |= _BV(PD7);
		_delay_ms (DEL);
		PORTD |= _BV(PD6);
		_delay_ms (DEL);
		PORTD |= _BV(PD5);
		_delay_ms (DEL);
		PORTB |= _BV(PB7);
		_delay_ms (DEL);
		PORTB |= _BV(PB6);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTB &= ~_BV(PB0);
		_delay_ms (DEL);
		PORTD &= ~_BV(PD7);
		_delay_ms (DEL);
		PORTD &= ~_BV(PD6);
		_delay_ms (DEL);
		PORTD &= ~_BV(PD5);
		_delay_ms (DEL);
		PORTB &= ~_BV(PB7);
		_delay_ms (DEL);
		PORTB &= ~_BV(PB6);
		_delay_ms (DEL);
	}
}

