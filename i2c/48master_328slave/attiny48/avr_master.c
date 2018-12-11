#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "avr_i2c.h"

static uint8_t i2c_state = I2C_NO_STATE;
static uint8_t i2c_buf[16];
static uint8_t i2c_buf_idx;
static uint8_t i2c_buf_len;

static bool i2c_msg_recv_good;
static bool i2c_msg_send_stop;


void
I2C_Master_Init (void)
{
	i2c_state = I2C_NO_STATE;
	i2c_msg_recv_good = false;
	i2c_msg_send_stop = false;
	i2c_buf_idx = 0;
	i2c_buf_len = 0;

	TWSR = 0x0; /* TWPS bits 0x0 gives a prescalar of 1 */
	TWBR = I2C_TWBR;
	TWDR = 0xff; /* SDA is released */
	TWCR = _BV(TWEN);
}


/*
bool
I2C_Transaction_Success (void)
{
	return i2c_msg_recv_good;
}
*/


bool
I2C_Busy (void)
{
	/* busy if TWI interrupt is enabled */
	return (TWCR & _BV(TWIE)) != 0x0;
}


uint8_t
I2C_State (void)
{
	while (I2C_Busy()) {}
	return i2c_state;
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
				/* last byte xmitted */
				i2c_msg_recv_good = true;
				TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			}
			break;

		case I2C_MRX_DATA_ACK:
			i2c_buf[i2c_buf_idx++] = TWDR;
		case I2C_MRX_ADR_ACK:
			if (i2c_buf_idx < (i2c_buf_len - 1))
			{
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			}
			else
			{
				/* NACK on last byte */
				TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
			}
			break;

		case I2C_MRX_DATA_NACK:
			i2c_buf[i2c_buf_idx] = TWDR;
			i2c_msg_recv_good = true;
			/* TWI interface enabled, disable TWI interrupt
			 * and clear the flag, initiate stop */
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		case I2C_ARB_LOST:
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWSTA);
			break;

		case I2C_MTX_ADR_NACK:
		case I2C_MRX_ADR_NACK:
		case I2C_MTX_DATA_NACK:
			i2c_state = TWSR & 0xfc;
			/* send stop to clear things out since slave NACK'd */
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
			break;

		case I2C_BUS_ERROR:
		case I2C_NO_STATE:
		default:
			/* reset TWI interface */
			i2c_state = TWSR & 0xfc;
			TWCR = _BV(TWEN);
			break;
	}
}
