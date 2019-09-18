#include <stdint.h>
#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "avr_i2c.h"
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

	while (1)
	{
		//I2C_Master_Write (100, (uint8_t *)"abcd", 4);
		//_delay_ms (5);

		button_pressed = (PIND & _BV(PD7)) == 0x0;
		//TODO: write button_pressed to slave

		//TODO: read button state from slave;
		//led_on = ??

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
