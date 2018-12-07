#include <stdint.h>

#include "avr_i2c.h"

volatile uint8_t i2c_state = I2C_NO_STATE;

static bool i2c_msg_recv_good;
static bool i2c_msg_send_stop;


void
I2C_Master_Init (void)
{
	TWSR = 0x0; /* TWPS bits 0x0 gives a prescalar of 1 */
	TWDR = 0xff; /* SDA is released */
	TWBR = I2C_TWBR;
	TWCR = _BV(TWEN);

	i2c_msg_recv_good = false;
	i2c_msg_send_stop = false;

	i2c_state = I2C_NO_STATE;
}


ISR(TWI_vect)
{
	switch (TWSR & 0xfc)
	{
		//...
		default:
			i2c_state = TWSR & 0xfc;
			/* reset the TWI Interface */
			TWCR = _BV(TWEN);
			break;
	}
}
