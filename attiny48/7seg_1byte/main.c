#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

#if 0
//#define L(X) (0)
//#define H(X) (_BV((X)))
//
//uint8_t led_map_B_lo[16] =
//{
//	/*   6        5        4        3        2        1        0 */
//	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | L(PB1) | L(PB0), /* 0 */
//	L(PB6) | L(PB5) | H(PB4) | L(PB3) | L(PB2) | L(PB1) | L(PB0), /* 1 */
//	H(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | H(PB1) | L(PB0), /* 2 */
//	H(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | H(PB1) | L(PB0), /* 3 */
//	L(PB6) | L(PB5) | H(PB4) | H(PB3) | L(PB2) | H(PB1) | L(PB0), /* 4 */
//	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 5 */
//	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 6 */
//	L(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | L(PB1) | L(PB0), /* 7 */
//	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 8 */
//	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 9 */
//	L(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* a */
//	H(PB6) | L(PB5) | L(PB4) | H(PB3) | L(PB2) | H(PB1) | L(PB0), /* b */
//	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | L(PB1) | L(PB0), /* c */
//	H(PB6) | L(PB5) | H(PB4) | L(PB3) | L(PB2) | H(PB1) | L(PB0), /* d */
//	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* e */
//	L(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* f */
//};
//
//uint8_t led_map_B_hi[16] =
//{
//	H(PB0) | H(PB5), /* 0 */
//	L(PB0) | H(PB5), /* 1 */
//	L(PB0) | H(PB5), /* 2 */
//	L(PB0) | H(PB5), /* 3 */
//	H(PB0) | H(PB5), /* 4 */
//	H(PB0) | L(PB5), /* 5 */
//	H(PB0) | L(PB5), /* 6 */
//	L(PB0) | H(PB5), /* 7 */
//	H(PB0) | H(PB5), /* 8 */
//	H(PB0) | H(PB5), /* 9 */
//	H(PB0) | H(PB5), /* a */
//	H(PB0) | L(PB5), /* b */
//	H(PB0) | L(PB5), /* c */
//	L(PB0) | H(PB5), /* d */
//	H(PB0) | L(PB5), /* e */
//	H(PB0) | L(PB5), /* f */
//};
//
//uint8_t led_map_D_lo[16] =
//{
//	H(PD5) | H(PD6), /* 0 */
//	L(PD5) | H(PD6), /* 1 */
//	H(PD5) | L(PD6), /* 2 */
//	L(PD5) | H(PD6), /* 3 */
//	L(PD5) | H(PD6), /* 4 */
//	L(PD5) | H(PD6), /* 5 */
//	H(PD5) | H(PD6), /* 6 */
//	L(PD5) | H(PD6), /* 7 */
//	H(PD5) | H(PD6), /* 8 */
//	L(PD5) | H(PD6), /* 9 */
//	H(PD5) | H(PD6), /* a */
//	H(PD5) | H(PD6), /* b */
//	H(PD5) | L(PD6), /* c */
//	H(PD5) | H(PD6), /* d */
//	H(PD5) | L(PD6), /* e */
//	H(PD5) | L(PD6), /* f */
//};
//
//uint8_t led_map_D_hi[16] =
//{
//	/*   6        5        4        3        2        1        0 */
//	L(PD6) | L(PD5) | H(PD4) | L(PD3) | H(PD2) | H(PD1) | H(PD0), /* 0 */
//	L(PD6) | L(PD5) | L(PD4) | L(PD3) | H(PD2) | L(PD1) | L(PD0), /* 1 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | H(PD0), /* 2 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 3 */
//	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | L(PD1) | L(PD0), /* 4 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 5 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* 6 */
//	L(PD6) | L(PD5) | H(PD4) | L(PD3) | H(PD2) | L(PD1) | L(PD0), /* 7 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* 8 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 9 */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | L(PD0), /* a */
//	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* b */
//	L(PD6) | L(PD5) | H(PD4) | L(PD3) | L(PD2) | H(PD1) | H(PD0), /* c */
//	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* d */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | H(PD0), /* e */
//	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | L(PD0), /* f */
//};
//
//
//static void
//SetLEDs (uint8_t lo, uint8_t hi)
//{
//	uint8_t x;
//
//	/* low nibble is mostly PORTB bits */
//	x = led_map_B_lo[lo];
//	/* but high nibble has 2 PORTB bits */
//	x |= led_map_B_hi[hi];
//	PORTB = x;
//
//	/* high nibble is mostly PORTD bits */
//	x = led_map_D_hi[hi];
//	/* but low nibble has 2 PORTD bits */
//	x |= led_map_D_lo[lo];
//	PORTD = x;
//}


static void
setup (void)
{
}


static void
run_draw_cycle (uint8_t value)
{
	for (uint8_t i = 0; i < 2; i++)
	{
		uint8_t is_low = (i == 0);
		if (is_low)
			; // drive low nibble select low
		else
			; // drive high nibble select low

		uint8_t nibval = (value >> (i * 4)) & 0xf;
		//...

		if (is_low)
			; // drive low nibble select high
		else
			; // drive high nibble select high
	}
}


int
main (void)
{
	uint8_t value = 0x0;
	setup ();
	while (1)
	{
		if (latch_pin_driven_low)
			value = 0x0; // #TODO: get correct val from pins
		run_draw_cycle (value);
	}
	return 0;
}

//		uint8_t c = PINC;
//		if (c & 0x3)
//		{
//			uint8_t nib = (c >> 2) & 0xf;
//			if (c & 0x1)
//				lo = nib;
//			if (c & 0x2)
//				hi = nib;
//			SetLEDs (lo, hi);
//		}
//		_delay_ms (1);
#endif


static void
setup (void)
{
	/* start off with all outputs */
	DDRB = 0xff;
	PORTB = 0x0;

	DDRC = 0xff;
	PORTC = 0x0;

	DDRD = 0xff;
	PORTD = 0x0;

#if 0
	/* data byte input */
	DDRB = 0x0;
	PORTB = 0xff; /* enable pull-ups */

	/* LED outputs */
	DDRD = 0xff;
	PORTD = 0x0; /* start low */

	DDRC = 0xff;
	PORTC = 0x0;

	/* latch data input; active low */
	DDRC &= ~_BV(PC0);
	PORTC |= _BV(PC0); /* enable pull-up */

	/* high nibble select output; drive low to select */
	PORTC |= _BV(PC1); /* start high */

	/* low nibble select output; drive low to select */
	PORTC |= _BV(PC2); /* start high */
#endif
}

#if 0
    0    1    2    3    4
    .    .    .    .    .


    .    .    .    .    .
    5    6    7    8    9

    2 - left cathode
    3 - right cathode

         0
       ______
      |      |
    4 |      | 1
      |  9   |
       ------
      |      |
    7 |      | 5
      |      |
       ------
         8     6 decimal

	0 - 014578
	1 - 15
	2 - 01789
	3 - 01589
	4 - 1459
	5 - 04589
	6 - 045789
	7 - 015
	8 - 0145789
	9 - 014589
	a - 014579
	b - 45789
	c - 789
	d - 15789
	e - 04789
	f - 0479
#endif

int
main (void)
{
	setup ();

	_delay_ms (50);

	if (1)
	{
		/* debug to check programming and timing */
		PORTB |= _BV(PB0); /* on */
		_delay_ms (100);
		PORTB &= ~_BV(PB0); /* off */
		_delay_ms (100);
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

