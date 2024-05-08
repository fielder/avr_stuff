#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	// outputs
	DDRC = _BV(PC7);

	// disable pull-ups
	PORTC = 0x0;

	// enable pull-ups
	PORTD |= _BV(PD2);

	//__builtin_avr_delay_cycles(1024);

	_delay_ms (50);
	while (1)
	{
		uint8_t pressed = !(PIND & _BV(PIND2));

		if (pressed)
			PORTC |= _BV(PC7);
		else
			PORTC &= ~_BV(PC7);
	}
}
