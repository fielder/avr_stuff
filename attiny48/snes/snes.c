#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>


void
_set (uint8_t bit, uint8_t onoff)
{
	if (onoff)
	{
		switch (bit)
		{
			case 0: PORTB |= _BV(PB0);
			case 1: PORTD |= _BV(PD7);
			case 2: PORTD |= _BV(PD6);
			case 3: PORTD |= _BV(PD5);
			case 4: PORTB |= _BV(PB7);
			case 5: PORTB |= _BV(PB6);
			case 6: PORTD |= _BV(PD4);
			case 7: PORTD |= _BV(PD3);
			case 8: PORTD |= _BV(PD2);
			case 9: PORTD |= _BV(PD1);
			case 10: PORTD |= _BV(PD0);
			case 11: PORTB |= _BV(PB1);
			default: break;
		}
	}
	else
	{
		switch (bit)
		{
			case 0: PORTB &= ~_BV(PB0);
			case 1: PORTD &= ~_BV(PD7);
			case 2: PORTD &= ~_BV(PD6);
			case 3: PORTD &= ~_BV(PD5);
			case 4: PORTB &= ~_BV(PB7);
			case 5: PORTB &= ~_BV(PB6);
			case 6: PORTD &= ~_BV(PD4);
			case 7: PORTD &= ~_BV(PD3);
			case 8: PORTD &= ~_BV(PD2);
			case 9: PORTD &= ~_BV(PD1);
			case 10: PORTD &= ~_BV(PD0);
			case 11: PORTB &= ~_BV(PB1);
			default: break;
		}
	}
}


int
main (void)
{
	uint8_t a = 0;
	uint8_t b = 1;
	uint8_t c = 2;

	DDRD =	_BV(PD0) |
		_BV(PD1) |
		_BV(PD2) |
		_BV(PD3) |
		_BV(PD4) |
		_BV(PD5) |
		_BV(PD6) |
		_BV(PD7);
	DDRB =	_BV(PB6) |
		_BV(PB7) |
		_BV(PB1) |
		_BV(PB0);

	_delay_ms (50);
	while (1)
	{
		uint8_t onoff[12];
		int i;
		for (i = 0; i < 12; i++)
		{
			if (i == a || i == b || i == c)
				onoff[i] = 1;
			else
				onoff[i] = 0;
		}
		for (i = 0; i < 12; i++)
		{
			if (onoff[i])
				_set (i, 1);
			else
				_set (i, 0);
		}
		_delay_ms (75);
		a = (a + 1) % 12;
		b = (b + 1) % 12;
		c = (c + 1) % 12;
	}
}
