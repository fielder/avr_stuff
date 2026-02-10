#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

/*
 * 2-digit 7-segment LED display pinout
 *
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

	 ___0___
	|       |
	4       1
	|___9___|
	|       |
	7       5
	|___8___|

		dec 6
*/

/* map to port D outputs
 * note this doesn't map directly to the display pins as LED pin 2 and 3
 * are cathodes
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


int
main (void)
{
	DDRC = 0xff; /* 8 outputs */
	PORTC = 0x0; /* start low */

	/* input; latch; active low */
// TODO: select latch pin; solder it up
//	DDRC &= ~_BV(PC0);
//	PORTC |= _BV(PC0); /* enable pull-up */

	DDRD = 0xff; /* 8 outputs */
	PORTD = 0x0; /* start low */

	/* input data byte */
	DDRB = 0x00; /* 8 inputs */
	PORTB = 0xff; /* enable pull-ups */

	_delay_ms (50);

	uint32_t clk = 0;
	uint8_t val = 0;

	/* start off w/ low nibble selected */
	PORTC |= _BV(PC4);

	for (uint8_t i = 0; i < 8; i++)
		SetOutputLED (i, 0);

#define USE_INPUTS 1
	while (1)
	{
		if (USE_INPUTS) {
			uint8_t latch_is_low = 0;

			if (1) {
				// TODO: read latch pin to trigger a read; active low
				latch_is_low = 1;
			}

			if (latch_is_low)
			{
				uint8_t in[8];
				const uint8_t pin = PINB;
				in[0] = (pin & _BV(PB0)) != 0x0;
				in[1] = (pin & _BV(PB1)) != 0x0;
				in[2] = (pin & _BV(PB2)) != 0x0;
				in[3] = (pin & _BV(PB3)) != 0x0;
				in[4] = (pin & _BV(PB4)) != 0x0;
				in[5] = (pin & _BV(PB5)) != 0x0;
				in[6] = (pin & _BV(PB6)) != 0x0;
				in[7] = (pin & _BV(PB7)) != 0x0;
				val = 0x0;
				for (uint8_t i = 0; i < 8; i++)
					val |= in[i] << i;
			}
		}

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
		if (!USE_INPUTS) {
			if ((clk & 0xff) == 0x0)
				val += 1;
		}
	}
	return 0;
}
