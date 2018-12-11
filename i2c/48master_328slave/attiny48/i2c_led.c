#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "avr_i2c.h"

extern void
I2C_Master_Init (void);

int
main (void)
{
	bool button_pressed = false;
	bool led_on = false;

	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	I2C_Master_Init ();

	sei ();

	_delay_ms (50);

	while (1)
	{
		//TODO: read button status from slave over TWI

		if (button_pressed)
			led_on = true;
		else
			led_on = false;

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
