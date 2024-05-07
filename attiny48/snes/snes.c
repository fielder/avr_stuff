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
	// set output pins
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
	DDRC =	_BV(PC5) |
		_BV(PC3);

	// PC5 - data clock
	// PC4 - data in
	// PC3 - data latch

	// enable pull-ups
	PORTB |=	_BV(PB5) |
			_BV(PB4) |
			_BV(PB3) |
			_BV(PB2);
	PORTC |=	_BV(PC4) |
			_BV(PC2) |
			_BV(PC1) |
			_BV(PC0) |
			_BV(PC7);

	_delay_ms (50);
	uint8_t scroll = 0;
	while (1)
	{
		uint8_t onoff[12];
		int i;
		for (i = 0; i < 12; i++)
		{
			// input = PINC & _BV(PINC4);
			onoff[i] = i == scroll;
		}

		for (i = 0; i < 12; i++)
			_set (i, onoff[i]);

		if (scroll == 0)
			PORTC |= _BV(PC3);
		else
			PORTC &= ~_BV(PC3);

		scroll = (scroll + 1) % 12;

		// _delay_ms (75);
	}
}
