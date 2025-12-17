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
	all_leds_on();
	_delay_ms(2000);
	all_leds_off();
    sei();
}

void take_input()
{
    unsigned char header, payload;
    if (!receive(&header, &payload))
        return;

    switch (header)
    {
    case ECHO:
        send(ECHO_REPLY, payload);
        break;
    
    default:
		error(UNKNOWN_COMMAND);
		send(RECEIVED_HEADER, header);
		turn_on_led(1);
        break;
    }
}

ISR(USART_RX_vect)
{
	turn_on_led(3);
	isr_receive_serial();
	/*
    if (has_received)
    {
        error(BUFFER_FULL);
        receive_byte();
        receive_byte();
        receive_byte();
    }
    else
    {
        received_header = receive_byte();
        received_payload = receive_byte();
        if ((received_header ^ received_payload) == receive_byte()) // parity correct
        {
            has_received = 1;
        }
        else
        {
			turn_on_led(2);
            error(INCORRECT_PARITY);
        }
    }
	*/
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
		_delay_ms(1000);
    }

}