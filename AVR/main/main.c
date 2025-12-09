#include <avr/io.h>
#include "leds.h"
#include "serial.h"
#include "globals.h"

#define LED0 PD2

void setup()
{
    init_serial();
    init_leds();
    sei();
}

void take_input()
{
    char header, payload;
    if (!receive(&header, &payload))
        return;

    switch (header)
    {
    case ECHO:
        send(ECHO_REPLY, payload);
        break;
    
    default:
        break;
    }
}

void main ()
{
    setup();
    while (1)
    {
        take_input();
        switch (state)
        {
        case STATE_BLINKY:
            toggle_led(0);
            break;
        
        default:
            error(INVALID_STATE);
            break;
        }
    }
    _delay_ms(1000);

}