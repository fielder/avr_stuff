#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

int
main (void)
{
	DDRB = (1 << PB6) | (1 << PB7) | (1 << PB0);
	DDRD = (1 << PD5) | (1 << PD6) | (1 << PD7);
	_NOP();
	PORTB = 0x0;
	PORTD = 0x0;
	_NOP();
	_delay_ms (50);
	while (1)
	{
#define DEL 100
		/* turn all on, one-by-one */
		PORTB |= (1 << PB0);
		_delay_ms (DEL);
		PORTD |= (1 << PD7);
		_delay_ms (DEL);
		PORTD |= (1 << PD6);
		_delay_ms (DEL);
		PORTD |= (1 << PD5);
		_delay_ms (DEL);
		PORTB |= (1 << PB7);
		_delay_ms (DEL);
		PORTB |= (1 << PB6);
		_delay_ms (DEL);

		/* turn all off, one-by-one */
		PORTB &= ~(1 << PB0);
		_delay_ms (DEL);
		PORTD &= ~(1 << PD7);
		_delay_ms (DEL);
		PORTD &= ~(1 << PD6);
		_delay_ms (DEL);
		PORTD &= ~(1 << PD5);
		_delay_ms (DEL);
		PORTB &= ~(1 << PB7);
		_delay_ms (DEL);
		PORTB &= ~(1 << PB6);
		_delay_ms (DEL);
	}
}
