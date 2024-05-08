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

	PORTC |= _BV(PC5); // pulses are from positive to ground

	while (1)
	{
		// 128 - 62.5 Hz
		// 132 - 60.2 Hz
		// 134 - 59.5 Hz
		// 140 - 57.5 Hz
		__builtin_avr_delay_cycles((uint32_t)1024 * (uint32_t)132);

		// 100 - 12.45 us
		// 95 - 11.8 us
		// 97 - 12.10 us
		PORTC |= _BV(PC3);
		__builtin_avr_delay_cycles(97);
		PORTC &= ~_BV(PC3);

		// 6 us after lowering latch:
		// send 16 down pulses on clock signal
		__builtin_avr_delay_cycles(45);

		uint8_t onoff[12];
		int i;

		for (i = 0; i < 16; i++)
		{
			PORTC &= ~_BV(PC5);
			// sample input on falling edge
			if (i < 12)
				onoff[i] = !(PINC & _BV(PINC4));
			__builtin_avr_delay_cycles(45);

			// controller shifts data out on rising edge
			PORTC |= _BV(PC5);
			__builtin_avr_delay_cycles(45);
		}

		for (i = 0; i < 12; i++)
			_set (i, onoff[i]);
	}
}
