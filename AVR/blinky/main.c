#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

#define LED0 PD2

void main ()
{
	int is_on = 0;
	DDRB = ( 1 << LED0 ); // set led0 to output
	while (1)
	{
		is_on = 1 - is_on;
		PORTD = ( is_on << LED0 ); // toggle led0
		_delay_ms(1000);
	}

}