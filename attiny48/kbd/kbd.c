#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <stdint.h>

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

/* ATTiny48
 *
 * PC6  PC5
 * PD0  PC4
 * PD1  PC3
 * PD2  PC2
 * PD3  PC1
 * PD4  PC0
 * Vcc  Gnd
 * Gnd  PC7
 * PB6  AVcc
 * PB7  PB5
 * PD5  PB4
 * PD6  PB3
 * PD7  PB2
 * PB0  PB1
*/

#if 0
PB0
PB1
PB2
PB3
PB4
PB5
PB6
PB7

PC0
PC1
PC2
PC3
PC4
PC5
PC6
PC7

PD0 /* output */
PD1 /* output */
PD2 /* output */
PD3 /* input */
PD4 /* input */
PD5
PD6
PD7
#endif


/* LEDs
 *
 * PD6 PB3
 * PD7 PB2
 * PB0 PB1
 */
void
illuminate_led (uint8_t idx, uint8_t on_off)
{
	if (on_off)
	{
		switch (idx)
		{
			case 0: PORTD |= _BV(PD6); break;
			case 1: PORTD |= _BV(PD7); break;
			case 2: PORTB |= _BV(PB0); break;
			case 3: PORTB |= _BV(PB3); break;
			case 4: PORTB |= _BV(PB2); break;
			case 5: PORTB |= _BV(PB1); break;
			default: break;
		}
	}
	else
	{
		switch (idx)
		{
			case 0: PORTD &= ~_BV(PD6); break;
			case 1: PORTD &= ~_BV(PD7); break;
			case 2: PORTB &= ~_BV(PB0); break;
			case 3: PORTB &= ~_BV(PB3); break;
			case 4: PORTB &= ~_BV(PB2); break;
			case 5: PORTB &= ~_BV(PB1); break;
			default: break;
		}
	}
}


void
drive (uint8_t idx, uint8_t hi)
{
	if (hi)
	{
		switch (idx)
		{
			case 0: PORTD |= _BV(PD0); break;
			case 1: PORTD |= _BV(PD1); break;
			case 2: PORTD |= _BV(PD2); break;
			default: break;
		}
	}
	else
	{
		switch (idx)
		{
			case 0: PORTD &= ~_BV(PD0); break;
			case 1: PORTD &= ~_BV(PD1); break;
			case 2: PORTD &= ~_BV(PD2); break;
			default: break;
		}
	}
}


uint8_t
sense (uint8_t idx)
{
	uint8_t pressed = 0;
	switch (idx)
	{
		case 0: pressed = !(PIND & _BV(PIND3)); break;
		case 1: pressed = !(PIND & _BV(PIND4)); break;
		default: break;
	}
	return pressed;
}


int
main (void)
{
	/* outputs */
	DDRB =	_BV(PB0) |
		_BV(PB1) |
		_BV(PB2) |
		_BV(PB3) |
		_BV(PB4) |
		_BV(PB5) |
		_BV(PB6) |
		_BV(PB7);
	DDRC =	_BV(PC0) |
		_BV(PC1) |
		_BV(PC2) |
		_BV(PC3) |
		_BV(PC4) |
		_BV(PC5) |
		_BV(PC6) |
		_BV(PC7);
	DDRD =	_BV(PD0) |
		_BV(PD1) |
		_BV(PD2) |
		_BV(PD5) |
		_BV(PD6) |
		_BV(PD7);

	/* enable pull-ups on inputs */
	PORTD |=	_BV(PD3) |
			_BV(PD4);

	_delay_ms (50);

	uint8_t presses[6];
	for (uint8_t i = 0; i < 6; i++)
		presses[i] = 0;

	/* set up driving output pins */
	for (uint8_t drv = 0; drv < 3; drv++)
		drive (drv, 1);

	while (1)
	{
		uint8_t idx = 0;
		for (uint8_t drv = 0; drv < 3; drv++)
		{
			drive (drv, 0);
			for (uint8_t sns = 0; sns < 2; sns++)
				presses[idx++] = sense(sns);
			drive (drv, 1);
		}

		for (idx = 0; idx < 6; idx++)
			illuminate_led (idx, presses[idx]);
	}
}
