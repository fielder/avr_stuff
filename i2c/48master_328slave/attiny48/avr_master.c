#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#include "avr_i2c.h"
#include "avr_master.h"

uint8_t i2c_status;

static uint8_t i2c_buf[16];
static uint8_t i2c_buf_len;
static uint8_t i2c_buf_idx;
#define I2C_BUF_SIZE (sizeof(i2c_buf) / sizeof(i2c_buf[0]))


void
I2C_Master_Init (void)
{
	i2c_buf_len = 0;
	i2c_buf_idx = 0;
	i2c_status = 0x0;

	TWSR = 0x0; /* TWPS bits 0x0 gives a prescalar of 1 */
	TWBR = I2C_TWBR;
	TWDR = 0xff;
	TWCR = _BV(TWEN); /* enable and release TWI pins */
}


void
I2C_Master_Disable (void)
{
	i2c_buf_len = 0;
	i2c_buf_idx = 0;
	i2c_status = 0x0;

	TWDR = 0xff;
	TWCR = 0x0;
}


void
I2C_Master_Write (uint8_t slaveaddr, uint8_t *data, uint8_t cnt)
{
	if ((cnt + 1) > I2C_BUF_SIZE)
	{
		i2c_status = I2C_STATUS_ERROR;
	}
	else
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			i2c_buf[0] = slaveaddr << 1;
			for (uint8_t i = 0; i < cnt; i++)
				i2c_buf[i + 1] = data[i];
			i2c_buf_len = cnt + 1;
			i2c_buf_idx = 0;
			i2c_status = 0x0;
		}
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTA);
	}
}


ISR(TWI_vect)
{
	switch (TWSR & 0xfc)
	{
		case I2C_START: /* START has been xmitted */
		case I2C_REP_START: /* repeated START has been xmitted */
			i2c_buf_idx = 0;
		case I2C_MTX_ADR_ACK: /* slave received addr+rw byte, ACK received */
		case I2C_MTX_DATA_ACK: /* slave received data byte, ACK received */
			if (i2c_buf_idx < i2c_buf_len)
			{
				TWDR = i2c_buf[i2c_buf_idx++];
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			}
			else
			{
				/* last byte xmitted; write complete */
				i2c_status |= I2C_STATUS_WRITE_COMPLETE;
				TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			}
			break;

		case I2C_MRX_DATA_ACK:
			/* data byte received from slave and ACK xmitted from master */
			i2c_buf[i2c_buf_idx++] = TWDR;
		case I2C_MRX_ADR_ACK:
			if (i2c_buf_idx < (i2c_buf_len - 1))
				/* ACK */
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			else
				/* master received final byte; NACK */
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			break;

		case I2C_MRX_DATA_NACK:
			/* data received from slave and NACK xmitted */
			i2c_buf[i2c_buf_idx] = TWDR;
			i2c_status |= I2C_STATUS_READ_COMPLETE;
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		case I2C_ARB_LOST:
			/* initiate a (RE)START condition */
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTA);
			break;

		case I2C_MTX_ADR_NACK:
		case I2C_MRX_ADR_NACK:
		case I2C_MTX_DATA_NACK:
			i2c_status |= I2C_STATUS_SLAVE_NACKED;
			/* send stop to clear things out since slave NACK'd */
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		case I2C_BUS_ERROR:
		case I2C_NO_STATE:
		default:
			/* reset TWI interface */
			i2c_status = 0x80;
			I2C_Master_Init ();
			break;
	}
}
