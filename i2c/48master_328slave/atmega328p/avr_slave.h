#ifndef __AVR_SLAVE_H__
#define __AVR_SLAVE_H__

#include <stdint.h>

/* master wrote to slave; (bytes, count) */
extern void (*i2c_rx_callback) (uint8_t *, uint8_t);

/* what master reads from slave */
extern uint8_t *i2c_tx_buf;
extern uint8_t i2c_tx_buf_len;

extern void
I2C_Slave_Init (uint8_t addr, uint8_t twbr);

extern void
I2C_Slave_Disable (void);

#endif /* __AVR_SLAVE_H__ */
