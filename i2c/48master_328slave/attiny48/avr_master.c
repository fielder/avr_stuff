#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/twi.h>

#include "avr_i2c.h"
#include "avr_master.h"

uint8_t i2c_status;
uint8_t i2c_buf[32];
uint8_t i2c_buf_len;
uint8_t i2c_buf_idx;


void
I2C_Master_Init (void)
{
	i2c_buf_len = 0;
	i2c_buf_idx = 0;
	i2c_status = I2C_STATUS_NONE;

	TWSR = 0; /* TWPS bits 0x0 gives a prescalar of 1 */
	TWBR = I2C_TWBR;
	TWDR = 0xff;
	TWCR = _BV(TWEN); /* enable and release TWI pins */
}


void
I2C_Master_Disable (void)
{
	i2c_buf_len = 0;
	i2c_buf_idx = 0;
	i2c_status = I2C_STATUS_NONE;

	TWDR = 0xff;
	TWCR = 0x0;
}


#if 0
void
I2C_Master_Write (uint8_t slaveaddr, uint8_t *data, uint8_t cnt)
{
	/* note buf needs to hold slaveaddr+rw byte */
	if ((cnt + 1) > I2C_BUF_SIZE)
	{
		i2c_status = I2C_STATUS_BUFFER_ERROR;
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
			i2c_status = I2C_STATUS_NONE;
		}
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTA);
	}
}
#endif


void
I2C_Master_Read (uint8_t slaveaddr, uint8_t *dest, uint8_t bufsz)
{
	// 1st byte should be SLA+R
	i2c_buf[0] = (slaveaddr << 1) | 0x1;
	// set i2c_buf_len to what master wants to read
	//...
}


ISR(TWI_vect)
{
	switch (TWSR & 0xfc)
	{
		case I2C_START:
		case I2C_REP_START:
			i2c_status = I2C_STATUS_NONE;
			TWDR = i2c_buf[0];
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			break;

//		I2C_ARB_LOST:
//			shouldn't happen w/ single-master setup
//			break;

		case I2C_MRX_ADR_ACK:
			/* slave ACK'ed addressing, master prepares to receive data */
			i2c_buf_idx = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_MRX_ADR_NACK:
			/* slave didn't ACK the SLA+RW */
			i2c_status = I2C_STATUS_SLAVE_NACKED;
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		case I2C_MRX_DATA_ACK:
			i2c_buf[i2c_buf_idx++] = TWDR;
			if (i2c_buf_idx < (i2c_buf_len - 1))
				/* ACK to slave */
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			else
				/* NACK to slave */
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			break;

		case I2C_MRX_DATA_NACK:
			/* final byte received from slave */
			i2c_buf[i2c_buf_idx++] = TWDR;
			i2c_status = I2C_STATUS_READ_COMPLETE;
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		default:
			i2c_status = I2C_STATUS_BUS_ERROR;
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;
	}
}
