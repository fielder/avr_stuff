#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <util/twi.h>

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

	switch (TWSR)
	{
		case I2C_STX_ADR_ACK:
			tx_idx = 0;
		case I2C_STX_DATA_ACK:
			if (tx_idx < i2c_tx_buf_len)
				TWDR = i2c_tx_buf[tx_idx++];
			else
				TWDR = 0xff;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			i2c_busy = 1;
			break;

		case I2C_STX_DATA_NACK:
			/* end of transmission */
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			i2c_busy = 0;
			break;

		case I2C_SRX_ADR_DATA_NACK:
		case I2C_SRX_GEN_DATA_NACK:
		case I2C_STX_DATA_ACK_LAST_BYTE:
		case I2C_BUS_ERROR:
			/* release SCL+SDA pins */
			TWCR = _BV(TWSTO) | _BV(TWINT);
			break;

		default:
			/* unknown status; reset */
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			i2c_busy = 0;
			break;
	}
}

/* ================================================================== */

int
main (void)
{
	DDRB = _BV(PB0); /* output */
	PORTB = 0x0;

	DDRD = (0 << PD7); /* input */
	PORTD = 0xff; /* enable all pull-ups on port D */

	I2C_Slave_Init ();

	_delay_ms (50);

	while (1)
	{
		uint8_t button_pressed = (PIND & _BV(PD7)) == 0x0;

		i2c_tx_buf[0] = button_pressed;
		i2c_tx_buf_len = 1;

		if (button_pressed)
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);
	}
}
