#ifndef __AVR_SLAVE_H__
#define __AVR_SLAVE_H__

#include <stdint.h>

/* what master reads from slave */
extern uint8_t *i2c_tx_buf;
extern uint8_t i2c_tx_buf_len;

extern void
I2C_Slave_Init (uint8_t addr, uint8_t twbr);

#endif /* __AVR_SLAVE_H__ */
