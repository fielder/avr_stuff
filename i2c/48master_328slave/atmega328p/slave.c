#ifndef F_CPU
#error "F_CPU not defined"
#endif

#ifndef I2C_SLAVE_ADDR
#error "I2C_SLAVE_ADDR not defined"
#endif

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/cpufunc.h>

#include <util/delay.h>

#include "avr_slave.h"

#define I2C_FREQ 100000

/*
 * - 1 pin as a button input, pulled up
 * - 1 pin as an LED output
 */

static bool button_pressed = false;
static bool led_on = false;


int
main (void)
{
	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

	_delay_ms (50);

#define twbr ((F_CPU / (2UL * I2C_FREQ)) - 8UL)
	I2C_Slave_Init (I2C_SLAVE_ADDR, twbr);

	while (1)
	{
		button_pressed = (PIND & _BV(PD7)) == 0x0;

		if (1)
		{
			if (button_pressed)
				led_on = true;
			else
				led_on = false;
		}

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
