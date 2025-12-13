#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "serial.h"

#define LED0 PD2

state_t state = STATE_BLINKY;
char shadow_PORTB = 0;
char shadow_PORTC = 0;
char shadow_PORTD = 0;

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
		error(UNKNOWN_COMMAND);
        break;
    }
}

int main ()
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