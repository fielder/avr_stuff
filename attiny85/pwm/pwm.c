#ifndef F_CPU
#error F_CPU not defined
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int
main (void)
{
	uint8_t is_on = 0;

	DDRB = _BV(PB4) | _BV(PB3) | _BV(PB2) | _BV(PB1); /* set outputs */
	PORTB = 0x0;

// Fan PWM frequency: 21kHz - 28kHz (25kHz nominal)

// Pinout:
// 1 PB5 (PCINT5/_RESET_/ADC0/dW)         8 Vcc
// 2 PB3 (PCINT3/XTAL1/CLKI/_OC1B_/ADC3)  7 PB2 (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
// 3 PB4 (PCINT4/XTAL2/CLKO/OC1B/ADC2)    6 PB1 (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
// 4 Gnd                                  5 PB0 (MOSI/DI/SDA/AIN0/OC0A/_OC1A_/AREF/PCINT0)

// DDRx  - 0=input, 1=output
// PORTx - value if an output
//         if input, write 1 to enable pull-up
// PINx  - read input

// Port B bit 3: _OC1B_: Inverted Output Compare Match output: The PB3 pin can serve as an external output for the Timer/Counter1 Compare Match B when configured as an output (DDB3 set). The _OC1B_ pin is also the inverted output pin for the PWM mode timer function.
//
// Port B bit 4: OC1B: Output Compare Match output: The PB4 pin can serve as an external output for the Timer/Counter1 Compare Match B when configured as an output (DDB4 set). The OC1B pin is also the output pin for the PWM mode timer function.

// In PWM mode, OCR1A and OCR1B provide the data values against which the Timer Counter value is compared.  Upon compare match the PWM outputs (OC1A, OC1A, OC1B, OC1B) are generated.

// TCCR1 - timer/counter control register
// GTCCR - general timer/counter control register
// OCR1A - timer/counter1 output compare register A
// OCR1B - ?
// OCR1C - timer/counter1 output compare register C
// DTPS1 - timer/counter1 dead time prescaler register 1
// DT1A  - timer/counter1 dead time A

// Pin 3 - PB4 - OC1B - Timer/Counter1 Compare Match B Output

	//OCR1A = 199; whas dis?
	OCR1B = 10;
	OCR1C = 157;

	// OC1B cleared on compare match; inverted not connected
	GTCCR = (1 << PWM1B) | (1 << COM1B1) | (0 << COM1B0);
	TCCR1 = (0 << CS13) | (0 << CS12) | (1 << CS11) | (0 << CS10);

	sei ();

	_delay_ms (50);
	while (1)
	{
		if (is_on)
		{
			PORTB &= ~_BV(PB1);
			PORTB &= ~_BV(PB2);
			PORTB &= ~_BV(PB3);
			is_on = 0;
			_delay_ms (100);
		}
		else
		{
			PORTB |= _BV(PB1);
			PORTB |= _BV(PB2);
			PORTB |= _BV(PB3);
			is_on = 1;
			_delay_ms (100);
		}
	}
}
