#include <stdint.h>
#include <stdbool.h>

//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "avr/sleep.h"

#include "TWI_Slave.h"


static uint8_t TWI_state = TWI_NO_STATE;

// the largest message size that will be sent including address byte
//#define TWI_BUFFER_SIZE 4
//static uint8_t TWI_buf[TWI_BUFFER_SIZE];

// Number of bytes to be transmitted.
//static uint8_t TWI_msgSize = 0;

// This is true when the TWI is in the middle of a transfer
// and set to false when all bytes have been transmitted/received
// Also used to determine how deep we can sleep.
static bool TWI_busy = false;

//union TWI_statusReg_t TWI_statusReg;


/*
 * Call this function to set up the TWI slave to its initial standby
 * state. Remember to enable interrupts from the main application after
 * initializing the TWI. Pass both the slave address and the
 * requirements for triggering on a general call in the same byte. Use
 * e.g. this notation when calling this function: The TWI module is
 * configured to NACK on any requests. Use a TWI_Start_Transceiver
 * function to start the TWI.
*/
void
TWI_Slave_Initialise (uint8_t addr, uint8_t twbr)
{
	TWI_state = TWI_NO_STATE;
	TWBR = twbr;
	TWAR = (addr << 1) & 0xfe;
	TWCR =	(1 << TWEN) | /* Enable TWI-interface and release TWI pins. */
		(0 << TWIE) |
		(0 << TWINT) |
		(0 << TWEA) |
		(0 << TWSTA) |
		(0 << TWSTO) |
		(0 << TWWC);
	TWI_busy = false;
}


/*
 * Is the TWI_ISR busy transmitting?
 */
bool
TWI_Busy (void)
{
	return TWI_busy;
}


/*
 * Fetch the state information of the previous operation. The function
 * will hold execution (loop) until the TWI_ISR has completed with the
 * previous operation. If there was an error, then the function will
 * return the TWI State code.
*/
uint8_t
TWI_State (void)
{
	/* wait until twi has completed the transmission */
	while (TWI_Busy()) {}
	return TWI_state;
}


#if 0
/****************************************************************************
Call this function to send a prepared message, or start the Transceiver for reception. Include
a pointer to the data to be sent if a SLA+W is received. The data will be copied to the TWI buffer.
Also include how many bytes that should be sent. Note that unlike the similar Master function, the
Address byte is not included in the message buffers.
The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
then initialize the next operation and return.
****************************************************************************/
void
TWI_Start_Transceiver_With_Data (uint8_t *msg, uint8_t msgSize)
{
  uint8_t temp;

  while (TWI_Busy()) {}             // Wait until TWI is ready for next transmission.

  TWI_msgSize = msgSize;                        // Number of data to transmit.
  for (temp = 0; temp < msgSize; temp++)      // Copy data that may be transmitted if the TWI Master requests data.
  {
    TWI_buf[ temp ] = msg[ temp ];
  }
  TWI_statusReg.all = 0;
  TWI_state         = TWI_NO_STATE;
  TWCR = (1<<TWEN)|                             // TWI Interface enabled.
         (1<<TWIE)|(1<<TWINT)|                  // Enable TWI Interrupt and clear the flag.
         (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|       // Prepare to ACK next time the Slave is addressed.
         (0<<TWWC);                             //
  TWI_busy = true;
}
#endif


/*
Call this function to start the Transceiver without specifying new transmission data. Useful for restarting
a transmission, or just starting the transceiver for reception. The driver will reuse the data previously put
in the transceiver buffers. The function will hold execution (loop) until the TWI_ISR has completed with the
previous operation, then initialize the next operation and return.
*/
void
TWI_Start_Transceiver (void)
{
	/* wait until TWI is ready for next transmission */
	while (TWI_Busy()) {}

	TWI_statusReg.all = 0;
	TWI_state = TWI_NO_STATE;
	TWCR =	(1 << TWEN) |
		(1 << TWIE) |
		(1 << TWINT) |
		(1 << TWEA) |
		(0 << TWSTA) |
		(0 << TWSTO) | /* prepare to ACK next time the slave is addressed */
		(0 << TWWC);
	TWI_busy = false;
}


#if 0
/****************************************************************************
Call this function to read out the received data from the TWI transceiver buffer. I.e. first call
TWI_Start_Transceiver to get the TWI Transceiver to fetch data. Then Run this function to collect the
data when they have arrived. Include a pointer to where to place the data and the number of bytes
to fetch in the function call. The function will hold execution (loop) until the TWI_ISR has completed
with the previous operation, before reading out the data and returning.
If there was an error in the previous transmission the function will return the TWI State code.
****************************************************************************/
uint8_t
TWI_Get_Data_From_Transceiver (uint8_t *msg, uint8_t msgSize)
{
  uint8_t i;

  while (TWI_Busy()) {}             // Wait until TWI is ready for next transmission.

  if (TWI_statusReg.lastTransOK)               // Last transmission completed successfully.
  {
    for (i=0; i<msgSize; i++)                 // Copy data from Transceiver buffer.
    {
      msg[ i ] = TWI_buf[ i ];
    }
    TWI_statusReg.RxDataInBuf = FALSE;          // Slave Receive data has been read from buffer.
  }
  return TWI_statusReg.lastTransOK;
}
#endif


#if 0
/****************************************************************************
This function is the Interrupt Service Routine (ISR), and called when the TWI interrupt is triggered;
that is whenever a TWI event has occurred. This function should not be called directly from the main
application.
****************************************************************************/
ISR(TWI_vect)
{
  static uint8_t TWI_bufPtr;

  switch (TWSR)
  {
    case TWI_STX_ADR_ACK:            // Own SLA+R has been received; ACK has been returned
      TWI_bufPtr   = 0;                                 // Set buffer pointer to first data location
    case TWI_STX_DATA_ACK:           // Data byte in TWDR has been transmitted; ACK has been received
      TWDR = TWI_buf[TWI_bufPtr++];
      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
             (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to send byte
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           //
             (0<<TWWC);                                 //
      TWI_busy = true;
      break;
    case TWI_STX_DATA_NACK:          // Data byte in TWDR has been transmitted; NACK has been received.
                                     // I.e. this could be the end of the transmission.
      if (TWI_bufPtr == TWI_msgSize) // Have we transceived all expected data?
      {
        TWI_statusReg.lastTransOK = TRUE;               // Set status bits to completed successfully.
      }
      else                          // Master has sent a NACK before all data where sent.
      {
        TWI_state = TWSR;                               // Store TWI State as error message.
      }

      TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
             (1<<TWIE)|(1<<TWINT)|                      // Keep interrupt enabled and clear the flag
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Answer on next address match
             (0<<TWWC);                                 //

      TWI_busy = false;   // Transmit is finished, we are not busy anymore
      break;
    case TWI_SRX_GEN_ACK:            // General call address has been received; ACK has been returned

      TWI_statusReg.genAddressCall = TRUE;
    case TWI_SRX_ADR_ACK:            // Own SLA+W has been received ACK has been returned

                                                        // Don't need to clear TWI_S_statusRegister.generalAddressCall due to that it is the default state.
      TWI_statusReg.RxDataInBuf = TRUE;
      TWI_bufPtr   = 0;                                 // Set buffer pointer to first data location

                                                        // Reset the TWI Interrupt to wait for a new event.
      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
             (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to send byte
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Expect ACK on this transmission
             (0<<TWWC);
      TWI_busy = true;

      break;
    case TWI_SRX_ADR_DATA_ACK:       // Previously addressed with own SLA+W; data has been received; ACK has been returned
    case TWI_SRX_GEN_DATA_ACK:       // Previously addressed with general call; data has been received; ACK has been returned
      TWI_buf[TWI_bufPtr++]     = TWDR;
      TWI_statusReg.lastTransOK = TRUE;                 // Set flag transmission successful.
                                                        // Reset the TWI Interrupt to wait for a new event.
      TWCR = (1<<TWEN)|                                 // TWI Interface enabled
             (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interrupt and clear the flag to send byte
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send ACK after next reception
             (0<<TWWC);                                 //
      TWI_busy = true;
      break;
    case TWI_SRX_STOP_RESTART:       // A STOP condition or repeated START condition has been received while still addressed as Slave
                                                        // Enter not addressed mode and listen to address match
      TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
             (1<<TWIE)|(1<<TWINT)|                      // Enable interrupt and clear the flag
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Wait for new address match
             (0<<TWWC);                                 //

      TWI_busy = false;  // We are waiting for a new address match, so we are not busy

      break;
    case TWI_SRX_ADR_DATA_NACK:      // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
    case TWI_SRX_GEN_DATA_NACK:      // Previously addressed with general call; data has been received; NOT ACK has been returned
    case TWI_STX_DATA_ACK_LAST_BYTE: // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received
    case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
      TWI_state = TWSR;                 //Store TWI State as error message, operation also clears noErrors bit
      TWCR =   (1<<TWSTO)|(1<<TWINT);   //Recover from TWI_BUS_ERROR, this will release the SDA and SCL pins thus enabling other devices to use the bus
      break;
    default:
      TWI_state = TWSR;                                 // Store TWI State as error message, operation also clears the Success bit.
      TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
             (1<<TWIE)|(1<<TWINT)|                      // Keep interrupt enabled and clear the flag
             (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Acknowledge on any new requests.
             (0<<TWWC);                                 //

      TWI_busy = false; // Unknown status, so we wait for a new address match that might be something we can handle
  }
}
#endif
