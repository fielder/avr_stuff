#include <stdint.h>
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
	uint8_t button_pressed = 0;
	uint8_t led_on = 0;

	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	I2C_Master_Init ();

	sei ();

	_delay_ms (50);

	while (1)
	{
		//TODO: read button status from slave over TWI

		if (button_pressed)
			led_on = 1;
		else
			led_on = 0;

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
