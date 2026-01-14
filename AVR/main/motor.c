#include "globals.h"
#include <avr/io.h>
#include "motor.h"

#define PWM0 PD5
#define PWM1 PD6
#define ENABLE PB7

static uint8_t inited = 0;

static uint8_t get_motor_mode()
{
    return (PINC & (1 << MOTOR_MODE_BTN)) >> MOTOR_MODE_BTN;
}
void init_motor()
{
    DDRC &= ~(1 << MOTOR_MODE_BTN); // set motor mode jumper as input with pull-up
    shadow_PORTC |= 1 << MOTOR_MODE_BTN;
    PORTC = shadow_PORTC;
    stop_motor();
    DDRB |= 1 << ENABLE; // set enable pin as output
    TCCR0A |= (1 << WGM00); // phase correct pwm on motor pins (counter0), WGM01 and WGM02 left at 0
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // set/clear on up/downcounting (idk which is which)
    TCCR0B = 1; // no prescaling

    // set pins as output
    DDRD |= 1 << PWM0;
    DDRD |= 1 << PWM1;
    inited = 1;
}
void stop_motor()
{
    if (!get_motor_mode())
    {
        setpwm0(255);
        setpwm1(255);
    }
    else
    {
        setpwm0(0);
        setpwm1(0);
    }
}


void setpwm0(uint8_t power)
{
    if (!inited)
        error(UNINITIALIZED);
    OCR0B = power;
}
void setpwm1(uint8_t power)
{
    if (!inited)
        error(UNINITIALIZED);
    OCR0A = power;
}
void set_enable(uint8_t enable)
{
    if (enable)
        shadow_PORTB |= 1 << ENABLE;
    else
        shadow_PORTB &= ~(1 << ENABLE);
    PORTB = shadow_PORTB;
}
