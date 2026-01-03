#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

#define LED4 PB2
#define LED5 PB1
#define TIMER1_PRESCALING 64
#define SPD0 PD7
#define SPD1 PB0

void setup()
{
    DDRB |= 1 << LED4;
    DDRB |= 1 << LED5;

    // digital encoder
    // set pins to output with pull-up
    DDRD &= ~(1 << SPD0);
	PORTD = 1 << SPD0;
    DDRB &= ~(1 << SPD1);
	PORTB = 1 << SPD1;
    // enable interrupts
    PCICR |= (1 << PCIE2); // enable PCINT 23..16
    PCMSK2 = (1 << PCINT23); // enable only PCINT23
    PCICR |= (1 << PCIE0); // enable PCINT 7..0
    PCMSK0 = (1 << PCINT0); // enable only PCINT0
	// configure clock
    if (TIMER1_PRESCALING == 1)
        TCCR1B = (unsigned char) 1;
    else if (TIMER1_PRESCALING == 8)
        TCCR1B = (unsigned char) 2;
    else if (TIMER1_PRESCALING == 64)
        TCCR1B = (unsigned char) 3;
    else if (TIMER1_PRESCALING == 256)
        TCCR1B = (unsigned char) 4;
    else if (TIMER1_PRESCALING == 1024)
        TCCR1B = (unsigned char) 5;
}

void main ()
{
	while (1)
	{
		is_on = 1 - is_on;
		PORTD = ( is_on << LED0 ); // toggle led0
		_delay_ms(1000);
	}

}