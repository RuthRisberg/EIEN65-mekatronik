#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"
#include "serial.h"
#include "sensors.h"

#define LED0 PD2
#define NUM_LEDS 6
#define NUM_BTNS 3

state_t state = STATE_NONE;
char shadow_PORTB = 0;
char shadow_PORTC = 0;
char shadow_PORTD = 0;

void setup()
{
    init_serial();
    init_leds();
    init_sensors();

	all_leds_on();
	_delay_ms(2000);
	all_leds_off();

    sei();
}

static int is_blinking[NUM_LEDS];
static int is_reporting_btns[NUM_BTNS];
static int is_reporting_potentiometer = 0;
void take_input()
{
    unsigned char header, payload;
    if (!receive(&header, &payload))
        return;

    switch (header)
    {
    case SET_STATE_NONE:
        state = STATE_NONE;
        break;

    case GET_STATE:
        send(CURRENT_STATE, state);
        break;

    case START_BLINK_LED:
        if (payload >= NUM_LEDS)
            error(INDEX_OUT_OF_BOUNDS);
        else
            is_blinking[payload] = 1;
        break;

    case STOP_BLINK_LED:
        if (payload >= NUM_LEDS)
            error(INDEX_OUT_OF_BOUNDS);
        else
            is_blinking[payload] = 0;
        break;

    case START_REPORT_BTNS:
        if (payload >= NUM_BTNS)
            error(INDEX_OUT_OF_BOUNDS);
        else
            is_reporting_btns[payload] = 1;
        break;

    case STOP_REPORT_BTNS:
        
        if (payload >= NUM_BTNS)
            error(INDEX_OUT_OF_BOUNDS);
        else
            is_reporting_btns[payload] = 0;
        break;

    case START_REPORT_POTENTIOMETER:
        is_reporting_potentiometer = 1;
        break;

    case STOP_REPORT_POTENTIOMETER:
        is_reporting_potentiometer = 0;
        break;

    case READ_BTNS:
        report_btn(payload);
        break;

    case READ_POTENTIOMETER:
        report_potentiometer();
        break;

    case TURN_ON_LED:
        turn_on_led(payload);
        break;

    case TURN_OFF_LED:
        turn_off_led(payload);
        break;

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
}

void continuous_tasks()
{
    for (int i = 0; i < NUM_LEDS; i++)
        if (is_blinking[i])
            toggle_led(i);
    
    for (int i = 0; i < NUM_BTNS; i++)
        if (is_reporting_btns[i])
            report_btn(i);
    
    if (is_reporting_potentiometer)
        report_potentiometer();
}

int main ()
{
    setup();
    while (1)
    {
        take_input();
        continuous_tasks();
        switch (state)
        {
        case STATE_NONE:
            break;
        
        default:
            error(INVALID_STATE);
            break;
        }
		_delay_ms(1000);
    }

}