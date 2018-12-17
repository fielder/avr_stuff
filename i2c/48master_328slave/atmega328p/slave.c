#ifndef F_CPU
#error "F_CPU not defined"
#endif

#ifndef I2C_SLAVE_ADDR
#error "I2C_SLAVE_ADDR not defined"
#endif

#include <stdint.h>

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "avr_slave.h"

#define I2C_FREQ 100000

/*
 * - 1 pin as a button input, pulled up
 * - 1 pin as an LED output
 */

static uint8_t button_pressed = 0;
static uint8_t led_on = 0;


static void
Data_RX (uint8_t *bytes, uint8_t len)
{
}


int
main (void)
{
	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

	i2c_rx_callback = Data_RX;

	i2c_tx_buf = &button_pressed;
	i2c_tx_buf_len = 1;

#define twbr ((F_CPU / (2UL * I2C_FREQ)) - 8UL)
	I2C_Slave_Init (I2C_SLAVE_ADDR, twbr);

	_delay_ms (50);

	while (1)
	{
		button_pressed = (PIND & _BV(PD7)) == 0x0;

		if (1)
		{
			if (button_pressed)
				led_on = 1;
			else
				led_on = 0;
		}

		if (led_on)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
