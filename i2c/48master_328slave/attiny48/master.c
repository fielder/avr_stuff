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
	DDRB = 0xff; /* all outputs */
	PORTB = 0x0;

	DDRD = 0xff; /* all outputs */
	PORTD = 0x0;

	DDRD &= ~_BV(PD7); /* make pin an input */
	PORTD |= _BV(PD7); /* enable pull-up */

	I2C_Master_Init ();

	_delay_ms (50);

	static uint8_t reading = 0;
	static uint8_t button_state = 0;
	static uint8_t button_pressed = 0;
	static uint8_t button_released = 0;
	static uint8_t led_on = 0;
	static uint8_t countdown = 0;
	while (1)
	{
		uint8_t b = (PIND & _BV(PD7)) == 0x0;
		button_pressed = b && !button_state;
		button_released = !b && button_state;
		button_state = b;

		if (reading) PORTD |= _BV(PD5); else PORTD &= ~_BV(PD5);

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
					//TODO: probably some kind of error
					reading = 0;
				}
			}
			else
			{
				if (++countdown == 0)
				{
					reading = 1;
					I2C_Master_BeginRead (100);
				}
			}
		}

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
