#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

#include "avr_slave.h"
#include "avr_i2c.h"

/*
 * - 1 pin as a button input, pulled up
 * - 1 pin as an LED output
 */

static uint8_t button_pressed = 0;


int
main (void)
{
	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

	i2c_tx_buf = &button_pressed;
	i2c_tx_buf_len = 1;

	I2C_Slave_Init (I2C_SLAVE_ADDR, I2C_TWBR);

	_delay_ms (50);

	while (1)
	{
		button_pressed = (PIND & _BV(PD7)) == 0x0;

		if (button_pressed)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
