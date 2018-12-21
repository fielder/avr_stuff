#include <stdlib.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>

#include "avr_i2c.h"
#include "avr_slave.h"

uint8_t *i2c_tx_buf = NULL;
uint8_t i2c_tx_buf_len = 0;


void
I2C_Slave_Init (uint8_t addr, uint8_t twbr)
{
	TWBR = twbr;
	TWAR = addr << 1;
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
			if (i2c_tx_buf != NULL && tx_idx < i2c_tx_buf_len)
				TWDR = i2c_tx_buf[tx_idx++];
			else
				TWDR = 0xff;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		default:
			/* unknown status; reset */
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;
	}
}
