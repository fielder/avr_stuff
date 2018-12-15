#ifndef __AVR_MASTER_H__
#define __AVR_MASTER_H__

#include <stdint.h>
#include <stdbool.h>

extern uint8_t i2c_status;

#define I2C_STATUS_WRITE_COMPLETE	(1 << 0)
#define I2C_STATUS_READ_COMPLETE	(1 << 1)
#define I2C_STATUS_SLAVE_NACKED		(1 << 2)

#define I2C_STATUS_ERROR	((i2c_status & 0x80) != 0x0)
#define I2C_STATUS_ERROR_VAL	(i2c_status & 0x7f)

extern void
I2C_Master_Init (void);

extern void
I2C_Master_Disable (void);

extern void
I2C_Master_Write (uint8_t slaveaddr, uint8_t *data, uint8_t cnt);

#endif /* __AVR_MASTER_H__ */
