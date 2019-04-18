#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

#define L(X) (0)
#define H(X) (_BV((X)))

uint8_t led_map_B_lo[16] =
{
	/*   6        5        4        3        2        1        0 */
	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | L(PB1) | L(PB0), /* 0 */
	L(PB6) | L(PB5) | H(PB4) | L(PB3) | L(PB2) | L(PB1) | L(PB0), /* 1 */
	H(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | H(PB1) | L(PB0), /* 2 */
	H(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | H(PB1) | L(PB0), /* 3 */
	L(PB6) | L(PB5) | H(PB4) | H(PB3) | L(PB2) | H(PB1) | L(PB0), /* 4 */
	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 5 */
	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 6 */
	L(PB6) | L(PB5) | H(PB4) | L(PB3) | H(PB2) | L(PB1) | L(PB0), /* 7 */
	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 8 */
	H(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* 9 */
	L(PB6) | L(PB5) | H(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* a */
	H(PB6) | L(PB5) | L(PB4) | H(PB3) | L(PB2) | H(PB1) | L(PB0), /* b */
	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | L(PB1) | L(PB0), /* c */
	H(PB6) | L(PB5) | H(PB4) | L(PB3) | L(PB2) | H(PB1) | L(PB0), /* d */
	H(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* e */
	L(PB6) | L(PB5) | L(PB4) | H(PB3) | H(PB2) | H(PB1) | L(PB0), /* f */
};

uint8_t led_map_B_hi[16] =
{
	H(PB0) | H(PB5), /* 0 */
	L(PB0) | H(PB5), /* 1 */
	L(PB0) | H(PB5), /* 2 */
	L(PB0) | H(PB5), /* 3 */
	H(PB0) | H(PB5), /* 4 */
	H(PB0) | L(PB5), /* 5 */
	H(PB0) | L(PB5), /* 6 */
	L(PB0) | H(PB5), /* 7 */
	H(PB0) | H(PB5), /* 8 */
	H(PB0) | H(PB5), /* 9 */
	H(PB0) | H(PB5), /* a */
	H(PB0) | L(PB5), /* b */
	H(PB0) | L(PB5), /* c */
	L(PB0) | H(PB5), /* d */
	H(PB0) | L(PB5), /* e */
	H(PB0) | L(PB5), /* f */
};

uint8_t led_map_D_lo[16] =
{
	H(PD5) | H(PD6), /* 0 */
	L(PD5) | H(PD6), /* 1 */
	H(PD5) | L(PD6), /* 2 */
	L(PD5) | H(PD6), /* 3 */
	L(PD5) | H(PD6), /* 4 */
	L(PD5) | H(PD6), /* 5 */
	H(PD5) | H(PD6), /* 6 */
	L(PD5) | H(PD6), /* 7 */
	H(PD5) | H(PD6), /* 8 */
	L(PD5) | H(PD6), /* 9 */
	H(PD5) | H(PD6), /* a */
	H(PD5) | H(PD6), /* b */
	H(PD5) | L(PD6), /* c */
	H(PD5) | H(PD6), /* d */
	H(PD5) | L(PD6), /* e */
	H(PD5) | L(PD6), /* f */
};

uint8_t led_map_D_hi[16] =
{
	/*   6        5        4        3        2        1        0 */
	L(PD6) | L(PD5) | H(PD4) | L(PD3) | H(PD2) | H(PD1) | H(PD0), /* 0 */
	L(PD6) | L(PD5) | L(PD4) | L(PD3) | H(PD2) | L(PD1) | L(PD0), /* 1 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | H(PD0), /* 2 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 3 */
	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | L(PD1) | L(PD0), /* 4 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 5 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* 6 */
	L(PD6) | L(PD5) | H(PD4) | L(PD3) | H(PD2) | L(PD1) | L(PD0), /* 7 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* 8 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | L(PD1) | H(PD0), /* 9 */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | H(PD2) | H(PD1) | L(PD0), /* a */
	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* b */
	L(PD6) | L(PD5) | H(PD4) | L(PD3) | L(PD2) | H(PD1) | H(PD0), /* c */
	L(PD6) | L(PD5) | L(PD4) | H(PD3) | H(PD2) | H(PD1) | H(PD0), /* d */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | H(PD0), /* e */
	L(PD6) | L(PD5) | H(PD4) | H(PD3) | L(PD2) | H(PD1) | L(PD0), /* f */
};


static void
SetLEDs (uint8_t lo, uint8_t hi)
{
	uint8_t x;

	/* low nibble is mostly PORTB bits */
	x = led_map_B_lo[lo];
	/* but high nibble has 2 PORTB bits */
	x |= led_map_B_hi[hi];
	PORTB = x;

	/* high nibble is mostly PORTD bits */
	x = led_map_D_hi[hi];
	/* but low nibble has 2 PORTD bits */
	x |= led_map_D_lo[lo];
	PORTD = x;
}


int
main (void)
{
	DDRB = 0xff; /* outputs */
	DDRD = 0xff; /* outputs */

	DDRC = 0x00; /* inputs */
	PORTC = 0xff; /* enable pull-ups */

	_delay_ms (50);

	uint8_t lo = 0;
	uint8_t hi = 0;
	while (1)
	{
		uint8_t c = PINC;
		if (c & 0x3)
		{
			uint8_t nib = (c >> 2) & 0xf;
			if (c & 0x1)
				lo = nib;
			if (c & 0x2)
				hi = nib;
			SetLEDs (lo, hi);
		}
		_delay_ms (1);
	}
}

