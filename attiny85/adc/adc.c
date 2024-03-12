#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

// External Voltage Reference at PB0 (AREF) pin, Internal Voltage Reference turned off.
#define ADMUX_VREF_SELECT ((0 << REFS2) | (0 << REFS1) | (1 << REFS0))

// Write one to ADLAR to left adjust the result.
// use ADCH to get the ADC value
#define ADMUX_ADLAR_SELECT (1 << ADLAR)

// ADC1 (pin PB2)
#define ADMUX_ADC_INPUT_SELECT ((0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0))

// reduce power consumption since we're not using the pin for digital input
#define DIGITAL_INPUT_DISABLE_SELECT ((0 << ADC3D) | (0 << ADC2D) | (1 << ADC1D) | (0 << ADC0D))


int
main (void)
{
	DDRB = _BV(PB3); /* output */
	DDRB = _BV(PB4); /* output */
	PORTB = 0x0; /* disable pull-ups */

	ADMUX = ADMUX_VREF_SELECT | ADMUX_ADLAR_SELECT | ADMUX_ADC_INPUT_SELECT;
	// enable & aut-trigger the ADC
	ADCSRA = ((1 << ADEN) | (1 << ADATE));
	// ADC auto trigger mode: free-running mode
	ADCSRB = ((0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0));

	DIDR0 = ((0 << ADC3D) | (0 << ADC2D) | (1 << ADC1D) | (0 << ADC0D));

	_delay_ms (50);
	while (1)
	{
		// read ADC value
		uint32_t adch = ADCH;
		uint32_t adcl = ADCL; // read & discard; shouldn't be needed but do anyways to be safe
		uint32_t adcval = adch;

		/* turn all on, one-by-one */
		PORTB |= _BV(PB3);
		PORTB |= _BV(PB4);
		_delay_ms (250);

		/* turn all off, one-by-one */
		PORTB &= ~_BV(PB3);
		PORTB &= ~_BV(PB4);
		_delay_ms (250);
	}
}

