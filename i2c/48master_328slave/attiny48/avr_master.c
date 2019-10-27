#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/twi.h>

#include "avr_master.h"

#define I2C_SCL_FREQ 100000
#define I2C_TWBR ((F_CPU / (2UL * I2C_SCL_FREQ)) - 8UL)

uint8_t i2c_status;
uint8_t i2c_buf[16];
uint8_t i2c_buf_len;


void
I2C_Master_Init (void)
{
	i2c_buf_len = 0;
	i2c_status = I2C_STATUS_NONE;

	TWSR = 0; /* TWPS bits 0x0 gives a prescalar of 1 */
	TWBR = I2C_TWBR;
	TWDR = 0xff;
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA); /* enable and release TWI pins */

	sei ();
}


void
I2C_Master_Disable (void)
{
	i2c_buf_len = 0;
	i2c_status = I2C_STATUS_NONE;

	TWCR = 0x0;
}


static void
Stop (void)
{
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO);
	while (TWCR & _BV(TWSTO)) {}
}


static uint8_t i2c_slave_addr;

void
I2C_Master_BeginRead (uint8_t slaveaddr)
{
	i2c_status = I2C_STATUS_NONE;
	i2c_slave_addr = slaveaddr;
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTA);
}


ISR(TWI_vect)
{
	switch (TW_STATUS)
	{
		case TW_START:
		case TW_REP_START:
			TWDR = (i2c_slave_addr << 1) | TW_READ;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case TW_MR_ARB_LOST:
			i2c_status = I2C_STATUS_BUS_ERROR;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case TW_MR_SLA_ACK:
			/* slave ACK'ed addressing, master prepares to receive data */
			i2c_buf_len = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case TW_MR_SLA_NACK:
			/* slave didn't ACK the SLA+RW */
			i2c_status = I2C_STATUS_SLAVE_NACKED;
			Stop ();
			break;

		case TW_MR_DATA_ACK:
			if (i2c_buf_len < I2C_BUF_SIZE)
				i2c_buf[i2c_buf_len++] = TWDR;
			if (i2c_buf_len < I2C_BUF_SIZE)
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			else
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			break;

		case TW_MR_DATA_NACK:
			/* final byte received from slave */
			if (i2c_buf_len < I2C_BUF_SIZE)
				i2c_buf[i2c_buf_len++] = TWDR;
			i2c_status = I2C_STATUS_READ_COMPLETE;
			Stop ();
			break;

		default:
			i2c_status = I2C_STATUS_BUS_ERROR;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWSTO);
			break;
	}
}
