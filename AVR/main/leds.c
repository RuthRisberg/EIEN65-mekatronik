#include "globals.h"
#include <avr/io.h>
#include "leds.h"

#define LED0 PD2
#define LED1 PC5
#define LED2 PC2
#define LED3 PC4
#define LED4 PB2
#define LED5 PB1

static int8_t inited = 0;

void init_leds()
{
    inited = 1;
    DDRD |= 1 << LED0;
    DDRC |= 1 << LED1;
    DDRC |= 1 << LED2;
    DDRC |= 1 << LED3;
    DDRB |= 1 << LED4;
    DDRB |= 1 << LED5;
    all_leds_off();
}
void all_leds_off()
{
    if (!inited)
        error(UNINITIALIZED);
    for (int i = 0; i < 6; i++)
        turn_off_led(i);
}
void all_leds_on()
{
    if (!inited)
        error(UNINITIALIZED);
    for (uint8_t i = 0; i < 6; i++)
        turn_on_led(i);
}
void turn_on_led(uint8_t led)
{
    if (!inited)
        error(UNINITIALIZED);
    if (led > 5)
    {
        error(INDEX_OUT_OF_BOUNDS);
        return;
    }
    if (led == 0)
    {
        shadow_PORTD |= 1 << LED0;
        PORTD = shadow_PORTD;
    }
    else if (led < 4)
    {
        if (led == 1)
            shadow_PORTC |= 1 << LED1;
        else if (led == 2)
            shadow_PORTC |= 1 << LED2;
        else
            shadow_PORTC |= 1 << LED3;
        PORTC = shadow_PORTC;
    }
    else
    {
        if (led == 4)
            shadow_PORTB |= 1 << LED4;
        else
            shadow_PORTB |= 1 << LED5;
        PORTB = shadow_PORTB;
    }
}
void turn_off_led(uint8_t led)
{
    if (!inited)
        error(UNINITIALIZED);
    if (led > 5)
    {
        error(INDEX_OUT_OF_BOUNDS);
        return;
    }
    if (led == 0)
    {
        shadow_PORTD &= ~(1 << LED0);
        PORTD = shadow_PORTD;
    }
    else if (led < 4)
    {
        if (led == 1)
            shadow_PORTC &= ~(1 << LED1);
        else if (led == 2)
            shadow_PORTC &= ~(1 << LED2);
        else
            shadow_PORTC &= ~(1 << LED3);
        PORTC = shadow_PORTC;
    }
    else
    {
        if (led == 4)
            shadow_PORTB &= ~(1 << LED4);
        else
            shadow_PORTB &= ~(1 << LED5);
        PORTB = shadow_PORTB;
    }
}
void toggle_led(uint8_t led)
{
    if (!inited)
        error(UNINITIALIZED);
    if (led > 5)
    {
        error(INDEX_OUT_OF_BOUNDS);
        return;
    }
    if (led == 0)
    {
        shadow_PORTD ^= 1 << LED0;
        PORTD = shadow_PORTD;
    }
    else if (led < 4)
    {
        if (led == 1)
            shadow_PORTC ^= 1 << LED1;
        else if (led == 2)
            shadow_PORTC ^= 1 << LED2;
        else
            shadow_PORTC ^= 1 << LED3;
        PORTC = shadow_PORTC;
    }
    else
    {
        if (led == 4)
            shadow_PORTB ^= 1 << LED4;
        else
            shadow_PORTB ^= 1 << LED5;
        PORTB = shadow_PORTB;
    }
}
