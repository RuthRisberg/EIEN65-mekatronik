#include "globals.h"
#include <avr/io.h>
#include "motor.h"

#define PWM0 PD5
#define PWM1 PD6

static uint8_t inited = 0;

void init_motor()
{
    setpwm1(0);
    setpwm2(0);
    TCCR0A |= (1 << WGM00); // phase correct pwm on motor pins (counter0), WGM01 and WGM02 left at 0
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // clear pin on match when up-counting, set on match down-counting
    TCCR0B = 1; // no prescaling

    // set pins as output
    DDRD |= 1 << PWM0;
    DDRD |= 1 << PWM1;
    inited = 1;
}

void setpwm1(uint8_t power)
{
    if (!inited)
        error(UNINITIALIZED);
    OCR0A = power;
}
void setpwm2(uint8_t power)
{
    if (!inited)
        error(UNINITIALIZED);
    OCR0B = power;
}
