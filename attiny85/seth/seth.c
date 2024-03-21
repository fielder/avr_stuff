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
	uint8_t button_pressed = 0;

	DDRB = _BV(PB4) | _BV(PB3) | _BV(PB1); /* set outputs */
	PORTB = _BV(PB2);

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

#define PWM_MAX_VAL 157
	//OCR1A = 199; whas dis?
	OCR1B = 79; // duty cycle: 0 to OCR1C
	OCR1C = PWM_MAX_VAL;

	// OC1B cleared on compare match; inverted not connected
	GTCCR = (1 << PWM1B) | (1 << COM1B1) | (0 << COM1B0);

	TCCR1 = (0 << CS13) | (0 << CS12) | (1 << CS11) | (0 << CS10);

#if 0
	// disable digital input buffer on ADC input
	DIDR0 = (1 << ADC1D);

	ADMUX = (0 << REFS2) | (0 << REFS1) | (1 << REFS0) | // AREF voltage reference
		(0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0) | // ADC1 (PB2)
		(1 << ADLAR); // put top 8 bits in 1 register for simpler reading (discard bottom 2 bits)

	ADCSRA = (1 << ADEN) | // enable
		(1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // ADC scalar: 64
#endif

	sei ();

	_delay_ms (50);
	while (1)
	{
#if 0
		ADCSRA |= (1 << ADSC); // start ADC measurement
		while (ADCSRA & (1 << ADSC)) {}

		uint8_t adch = ADCH;
		float adc_percentage = adch / 255.0f;

		float pwmout = adc_percentage * (float)PWM_MAX_VAL;
		if (pwmout < 1)
			pwmout = 0;
		if (pwmout > PWM_MAX_VAL)
			pwmout = PWM_MAX_VAL;
		OCR1B = (uint8_t)pwmout;
#endif

		button_pressed = (PINB & _BV(PB2)) == 0x0;

		if (!button_pressed)
		{
			PORTB &= ~_BV(PB1);
			PORTB &= ~_BV(PB3);
			OCR1B = 1;
		}
		else
		{
			PORTB |= _BV(PB1);
			PORTB |= _BV(PB3);
			OCR1B = PWM_MAX_VAL - 1;
		}
	}
}
