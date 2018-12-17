#include <stdlib.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>
#include <util/atomic.h>

#include "avr_i2c.h"
#include "avr_slave.h"

static uint8_t i2c_busy = 0;

void (*i2c_rx_callback) (uint8_t *, uint8_t) = NULL;

uint8_t *i2c_tx_buf = NULL;
uint8_t i2c_tx_buf_len = 0;


void
I2C_Slave_Init (uint8_t addr, uint8_t twbr)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		i2c_busy = 0;
		TWBR = twbr;
		TWAR = (addr << 1) & 0xfe;
		TWDR = 0xff;
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
	}
}


void
I2C_Slave_Disable (void)
{
	TWCR = 0x0;
	TWDR = 0xff;
	i2c_busy = 0;
}


ISR(TWI_vect)
{
	static uint8_t tx_idx = 0;

	static uint8_t rx_buf[16];
#define RX_BUF_SZ (sizeof(rx_buf) / sizeof(rx_buf[0]))
	static uint8_t rx_idx = 0;

	switch (TWSR)
	{
		/* master beginning a read from slave */
		case I2C_STX_ADR_ACK:
			tx_idx = 0;
		case I2C_STX_DATA_ACK:
			if (i2c_tx_buf != NULL && tx_idx < i2c_tx_buf_len)
				TWDR = i2c_tx_buf[tx_idx++];
			else
				TWDR = 0xff;
			i2c_busy = 1;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_STX_DATA_NACK:
			/* consider it finished */
			i2c_busy = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_SRX_GEN_ACK:
		case I2C_SRX_ADR_ACK:
			/* master beginning a write to slave */
			rx_idx = 0;
			i2c_busy = 1;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_SRX_GEN_DATA_ACK:
		case I2C_SRX_ADR_DATA_ACK:
			if (rx_idx < RX_BUF_SZ)
				rx_buf[rx_idx++] = TWDR;
			i2c_busy = 1;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_SRX_STOP_RESTART:
			/* write to slave complete */
			if (i2c_rx_callback != NULL)
				i2c_rx_callback (rx_buf, rx_idx);
			i2c_busy = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
			break;

		case I2C_SRX_ADR_DATA_NACK:
		case I2C_SRX_GEN_DATA_NACK:
		case I2C_STX_DATA_ACK_LAST_BYTE:
		case I2C_BUS_ERROR:
			TWCR = _BV(TWSTO) | _BV(TWINT);
			break;

		default:
			/* unknown status; reset */
			i2c_busy = 0;
			TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);
			break;
	}
}
