#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <util/twi.h>
#include <util/atomic.h>

#include "avr_i2c.h"

uint8_t i2c_busy = 0;

uint8_t i2c_tx_buf[32];
uint8_t i2c_tx_buf_len = 0;


static void
I2C_Slave_Init (void)
{
	i2c_busy = 0;
	i2c_tx_buf_len = 0;
	TWBR = I2C_TWBR;
	TWAR = I2C_SLAVE_ADDR << 1;
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
}


ISR(TWI_vect)
{
	static uint8_t tx_idx = 0;

	switch (TWSR & 0xfc)
	{
		case I2C_STX_ADR_ACK:
			tx_idx = 0;
			i2c_busy = 1;
		case I2C_STX_DATA_ACK:
			if (tx_idx < i2c_tx_buf_len)
			{
				TWDR = i2c_tx_buf[tx_idx++];
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			}
			else
			{
				TWDR = 0xff;
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			}
			break;

		case I2C_STX_DATA_NACK:
		case I2C_STX_DATA_ACK_LAST_BYTE:
			/* end of transmission */
			i2c_busy = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		default:
			/* unknown status; reset */
			i2c_busy = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;
	}
}

/* ================================================================== */

int
main (void)
{
	DDRB = 0xff; /* all outputs */
	PORTB = 0x0;

	DDRD = 0xff; /* all outputs */
	PORTD = 0x0;

	DDRD &= ~_BV(PD7); /* make pin an input */
	PORTD |= _BV(PD7); /* enable pull-up */

	I2C_Slave_Init ();

	_delay_ms (50);

	while (1)
	{
		if (i2c_busy)
			PORTD |= _BV(PD5);
		else
			PORTD &= ~_BV(PD5);

		uint8_t button_pressed = (PIND & _BV(PD7)) == 0x0;

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			if (!i2c_busy)
			{
				i2c_tx_buf[0] = button_pressed;
				i2c_tx_buf_len = 1;
			}
		}

		if (button_pressed)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
