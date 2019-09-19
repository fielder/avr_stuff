#include <stdint.h>

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <util/atomic.h>

#include "avr_master.h"


int
main (void)
{
	uint8_t button_pressed = 0;
	uint8_t led_on = 0;

	DDRB = 0xff; /* all outputs */
	PORTB = 0x0;

	DDRD = 0xff; /* all outputs */
	PORTD = 0x0;

	DDRD &= ~_BV(PD7); /* make pin an input */
	PORTD |= _BV(PD7); /* enable pull-up */

	I2C_Master_Init ();

	_delay_ms (50);

	static uint8_t reading = 0;
	while (1)
	{
		button_pressed = (PIND & _BV(PD7)) == 0x0;

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (reading)
			{
				if (i2c_status == I2C_STATUS_NONE)
				{
					/* working */
				}
				else if (i2c_status == I2C_STATUS_READ_COMPLETE)
				{
					led_on = i2c_buf[0];
					reading = 0;
				}
				else
				{
					reading = 0;
				}
			}
			else
			{
				if (button_pressed)
				{
					reading = 1;
					I2C_Master_Read (100);
				}
			}
		}

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
