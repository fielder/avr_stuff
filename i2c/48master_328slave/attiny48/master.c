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

	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

	I2C_Master_Init ();

	sei ();

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
