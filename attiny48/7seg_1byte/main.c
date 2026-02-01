#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

/*
	0    1    2    3    4
	.    .    .    .    .

	.    .    .    .    .
	5    6    7    8    9

	0 - t
	1 - tr
	2 - left cathode
	3 - right cathode
	4 - tl
	5 - br
	6 - decimal
	7 - bl
	8 - b
	9 - mid
*/

uint8_t LED_MAP[16][8] = {
/*	 0  1  2  3  4  5  6  7 */
	{1, 1, 1, 1, 0, 1, 1, 0}, // 0
	{0, 1, 0, 1, 0, 0, 0, 0}, // 1
	{1, 1, 0, 0, 0, 1, 1, 1}, // 2
	{1, 1, 0, 1, 0, 0, 1, 1}, // 3
	{0, 1, 1, 1, 0, 0, 0, 1}, // 4
	{1, 0, 1, 1, 0, 0, 1, 1}, // 5
	{1, 0, 1, 1, 0, 1, 1, 1}, // 6
	{1, 1, 0, 1, 0, 0, 0, 0}, // 7
	{1, 1, 1, 1, 0, 1, 1, 1}, // 8
	{1, 1, 1, 1, 0, 0, 1, 1}, // 9
	{1, 1, 1, 1, 0, 1, 0, 1}, // a
	{0, 0, 1, 1, 0, 1, 1, 1}, // b
	{0, 0, 0, 0, 0, 1, 1, 1}, // c
	{0, 1, 0, 1, 0, 1, 1, 1}, // d
	{1, 0, 1, 0, 0, 1, 1, 1}, // e
	{1, 0, 1, 0, 0, 1, 0, 1}, // f
};


static void
_SetLED (uint8_t led_idx, uint8_t onoff)
{
	switch (led_idx)
	{
		case 0: if (onoff) { PORTD |= _BV(PD0); } else { PORTD &= ~_BV(PD0); } break;
		case 1: if (onoff) { PORTD |= _BV(PD1); } else { PORTD &= ~_BV(PD1); } break;
		case 2: if (onoff) { PORTD |= _BV(PD2); } else { PORTD &= ~_BV(PD2); } break;
		case 3: if (onoff) { PORTD |= _BV(PD3); } else { PORTD &= ~_BV(PD3); } break;
		case 4: if (onoff) { PORTD |= _BV(PD4); } else { PORTD &= ~_BV(PD4); } break;
		case 5: if (onoff) { PORTD |= _BV(PD5); } else { PORTD &= ~_BV(PD5); } break;
		case 6: if (onoff) { PORTD |= _BV(PD6); } else { PORTD &= ~_BV(PD6); } break;
		case 7: if (onoff) { PORTD |= _BV(PD7); } else { PORTD &= ~_BV(PD7); } break;
		default: break;
	}
}


static uint8_t led_states[8] = {0, 0, 0, 0, 0, 0, 0, 0};

static void
SetOutputLED (uint8_t led_idx, uint8_t onoff)
{
	if (onoff)
	{
		if (!led_states[led_idx])
		{
			_SetLED (led_idx, 1);
			led_states[led_idx] = 1;
		}
	}
	else
	{
		if (led_states[led_idx])
		{
			_SetLED (led_idx, 0);
			led_states[led_idx] = 0;
		}
	}
}


static void
setup (void)
{
	DDRC = 0xff; /* 8 outputs */
	PORTC = 0x0; /* start low */

	/* input; latch; active low */
//	DDRC &= ~_BV(PC0);
//	PORTC |= _BV(PC0); /* enable pull-up */

	DDRD = 0xff; /* 8 outputs */
	PORTD = 0x0; /* start low */

	/* data byte */
	DDRB = 0x00; /* 8 inputs */
	PORTB = 0xff; /* enable pull-ups */
}


int
main (void)
{
	setup ();

	_delay_ms (50);

	if (1)
	{
		uint32_t clk = 0;
		uint8_t val = 0;

		/* start off w/ low nibble selected */
		PORTC |= _BV(PC4);

		for (uint8_t i = 0; i < 8; i++)
			SetOutputLED (i, 0);

		while (1)
		{
			PORTC &= ~_BV(PC5);
			PORTC |= _BV(PC4); /* select low nibble */
			for (uint8_t ledidx = 0; ledidx < 8; ledidx++)
				SetOutputLED (ledidx, LED_MAP[(val >> 0) & 0xf][ledidx]);
			_delay_ms (1); /* give a bit for the LEDs to show */

			PORTC |= _BV(PC5); /* select high nibble */
			PORTC &= ~_BV(PC4);
			for (uint8_t ledidx = 0; ledidx < 8; ledidx++)
				SetOutputLED (ledidx, LED_MAP[(val >> 4) & 0xf][ledidx]);
			_delay_ms (1); /* give a bit for the LEDs to show */

			clk += 1;
			if ((clk & 0xff) == 0x0)
				val += 1;
		}
	}
	else
	{
		// non-debug stuff
		while (1)
		{
		}
	}
	return 0;
}

