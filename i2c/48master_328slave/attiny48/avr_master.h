#ifndef __AVR_MASTER_H__
#define __AVR_MASTER_H__

#include <stdint.h>

extern uint8_t i2c_status;
extern uint8_t i2c_buf[16];
extern uint8_t i2c_buf_len;
#define I2C_BUF_SIZE (sizeof(i2c_buf) / sizeof(i2c_buf[0]))

#define I2C_STATUS_NONE			0
#define I2C_STATUS_WRITE_COMPLETE	(1 << 0)
#define I2C_STATUS_READ_COMPLETE	(1 << 1)

#define I2C_STATUS_SLAVE_NACKED		(0x80 + 1)
#define I2C_STATUS_BUS_ERROR		(0x80 + 2)
#define I2C_STATUS_BUFFER_ERROR		(0x80 + 3)

#define i2c_status_error	((i2c_status & 0x80) != 0x0)
#define i2c_status_error_val	(i2c_status & 0x7f)

extern void
I2C_Master_Init (void);

extern void
I2C_Master_Disable (void);

extern void
I2C_Master_BeginRead (uint8_t slaveaddr);

#endif /* __AVR_MASTER_H__ */
