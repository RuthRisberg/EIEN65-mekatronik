#include <avr/io.h>
#include "sensors.h"
#include "shared_enums.h"
#include "globals.h"

#define BTN0 PD4
#define BTN1 PB6
#define BTN2 PC1
#define POT ADC3 // PC3

void init_sensors()
{
    inited = 1;

    // just for clarity, these should already be 0
    PORTD &= ~(1 << BTN0);
    PORTB &= ~(1 << BTN1);
    PORTC &= ~(1 << BTN2);

    ADCSRA |= (1 << ADEN); // ADC enable
    ADMUX |= (1 << ADLAR); // left adjust result (most significant 8 bits in ADCH)
    ADMUX |= POT; // set the correct pin to read from
}

void report_btn(int btn)
{
    send(BTN_STATUS, (((unsigned char)btn)<<4) | read_button(btn));
}

void report_potentiometer()
{
    send(POTENTIOMETER_STATUS, read_potentiometer());
}

int read_button(int btn)
{
    switch (btn)
    {
    case 0:
        if (PIND & (1 << BTN0))
            return 1;
        else:
            return 0;
        break;
    case 1:
        if (PINB & (1 << BTN1))
            return 1;
        else:
            return 0;
        break;
    case 2:
        if (PINC & (1 << BTN2))
            return 1;
        else:
            return 0;
        break;
    
    default:
        error(INDEX_OUT_OF_BOUNDS);
        break;
    }
}

unsigned char read_potentiometer()
{
    ADCSRA |= (1 << ADSC); // ADC start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADCH;
}
