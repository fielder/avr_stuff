#ifndef __AVR_TWI_H__
#define __AVR_TWI_H__

#define I2C_SCL_FREQ 100000
#define I2C_TWBR ((F_CPU / (2UL * I2C_SCL_FREQ)) - 8UL)

enum twi_state_s
{
/* TWI master general staus codes */

 	/* START has been transmitted */
	I2C_START			= 0x08,

 	/* Repeated START has been transmitted */
	I2C_REP_START			= 0x10,

 	/* Arbitration lost */
	I2C_ARB_LOST			= 0x38,

/* TWI master transmitter staus codes */

 	/* SLA+W has been tramsmitted and ACK received */
	I2C_MTX_ADR_ACK			= 0x18,

 	/* SLA+W has been tramsmitted and NACK received */
	I2C_MTX_ADR_NACK		= 0x20,

 	/* Data byte has been tramsmitted and ACK received */
	I2C_MTX_DATA_ACK		= 0x28,

 	/* Data byte has been tramsmitted and NACK received */
	I2C_MTX_DATA_NACK		= 0x30,

/* TWI master receiver staus codes */

 	/* SLA+R has been tramsmitted and ACK received */
	I2C_MRX_ADR_ACK			= 0x40,

 	/* SLA+R has been tramsmitted and NACK received */
	I2C_MRX_ADR_NACK		= 0x48,

 	/* Data byte has been received and ACK tramsmitted */
	I2C_MRX_DATA_ACK		= 0x50,

 	/* Data byte has been received and NACK tramsmitted */
	I2C_MRX_DATA_NACK		= 0x58,

/* TWI slave transmitter staus codes */

 	/* Own SLA+R has been received; ACK has been returned */
	I2C_STX_ADR_ACK			= 0xa8,

 	/* Arbitration lost in SLA+R/W as Master; own SLA+R has been
	 * received; ACK has been returned */
	I2C_STX_ADR_ACK_M_ARB_LOST	= 0xb0,

 	/* Data byte in TWDR has been transmitted; ACK has been received
	 */
	I2C_STX_DATA_ACK		= 0xb8,

 	/* Data byte in TWDR has been transmitted; NOT ACK has been
	 * received */
	I2C_STX_DATA_NACK		= 0xc0,

 	/* Last data byte in TWDR has been transmitted; ACK has been
	 * received */
	I2C_STX_DATA_ACK_LAST_BYTE	= 0xc8,

/* TWI slave receiver staus codes */

 	/* Own SLA+W has been received ACK has been returned */
	I2C_SRX_ADR_ACK			= 0x60,

 	/* Arbitration lost in SLA+R/W as Master; own SLA+W has been
	 * received; ACK has been returned */
	I2C_SRX_ADR_ACK_M_ARB_LOST	= 0x68,

 	/* General call address has been received; ACK has been
	 * returned */
	I2C_SRX_GEN_ACK			= 0x70,

 	/* Arbitration lost in SLA+R/W as Master; General call address
	 * has been received; ACK has been returned */
	I2C_SRX_GEN_ACK_M_ARB_LOST	= 0x78,

 	/* Previously addressed with own SLA+W; data has been received;
	 * ACK has been returned */
	I2C_SRX_ADR_DATA_ACK		= 0x80,

 	/* Previously addressed with own SLA+W; data has been received;
	 * NOT ACK has been returned */
	I2C_SRX_ADR_DATA_NACK		= 0x88,

 	/* Previously addressed with general call; data has been
	 * received; ACK has been returned */
	I2C_SRX_GEN_DATA_ACK		= 0x90,

 	/* Previously addressed with general call; data has been
	 * received; NOT ACK has been returned */
	I2C_SRX_GEN_DATA_NACK		= 0x98,

 	/* A STOP condition or repeated START condition has been
	 * received while still addressed as Slave */
	I2C_SRX_STOP_RESTART		= 0xa0,

/* TWI miscellaneous status codes */

 	/* No relevant state information available; */
	I2C_NO_STATE			= 0xf8,

 	/* Bus error due to an illegal START or STOP condition */
	I2C_BUS_ERROR			= 0x00,
};

#endif /* __AVR_TWI_H__ */
