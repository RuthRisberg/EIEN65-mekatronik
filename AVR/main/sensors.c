#include <avr/io.h>
#include "sensors.h"
#include "shared_enums.h"
#include "globals.h"
#include "serial.h"

#define BTN0 PD4
#define BTN1 PB6
#define BTN2 PC1
#define POT ((unsigned char) 3) // PC3 ADC3
#define SPD0 PD7
#define READSPD0 (PIND & (1 << SPD0))
#define SPD1 PB0
#define READSPD1 (PINB & (1 << SPD1))

#define TRIGGERS_PER_ROTATION 24
#define CLOCK_CYCLES_PER_MINUTE (F_CPU*60)
#define PRESCALING 8
#define SPEED_CALC_FACTOR (10 * CLOCK_CYCLES_PER_MINUTE / PRESCALING / TRIGGERS_PER_ROTATION)

static int inited = 0;

// for speed calculations
static uint16_t last_trigger = 0;
static int16_t recent_times[10] = {0,0,0,0,0,0,0,0,0,0};
static int16_t avg_time = 0;
static int time_index = 0;
static uint8_t last_trigger_source = 0;

void init_sensors()
{
    // buttons
    DDRD &= ~(1 << BTN0);
	shadow_PORTD |= 1 << BTN0;
	PORTD = shadow_PORTD;
    DDRB &= ~(1 << BTN1);
	shadow_PORTB |= 1 << BTN1;
	PORTB = shadow_PORTB;
    DDRC &= ~(1 << BTN2);
	shadow_PORTC |= 1 << BTN2;
	PORTC = shadow_PORTC;

    // potentiometer
    ADMUX |= (1 << ADLAR); // left adjust result (most significant 8 bits in ADCH)
    ADMUX |= POT; // set the correct pin to read from
    ADCSRA |= (1 << ADEN); // ADC enable

    // digital encoder
    // set pins to output with pull-up
    DDRD &= ~(1 << SPD0);
	shadow_PORTD |= 1 << SPD0;
	PORTD = shadow_PORTD;
    DDRB &= ~(1 << SPD1);
	shadow_PORTB |= 1 << SPD1;
	PORTB = shadow_PORTB;
    // enable interrupts
    PCICR |= (1 << PCIE2); // enable PCINT 23..16
    PCMSK2 = (1 << PCINT23); // enable only PCINT23
    PCICR |= (1 << PCIE0); // enable PCINT 7..0
    PCMSK0 = (1 << PCINT0); // enable only PCINT0
	// configure clock
	//TCCR1B = (unsigned char) 1; // no prescaling
	TCCR1B = (unsigned char) 2; // 8x prescaling

    inited = 1;
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
	if (!inited)
		error(UNINITIALIZED);
    switch (btn)
    {
    case 0:
        if (PIND & (1 << BTN0))
            return 1;
        else
            return 0;
        break;
    case 1:
        if (PINB & (1 << BTN1))
            return 1;
        else
            return 0;
        break;
    case 2:
        if (PINC & (1 << BTN2))
            return 1;
        else
            return 0;
        break;
    
    default:
        error(INDEX_OUT_OF_BOUNDS);
		return 0;
        break;
    }
}

unsigned char read_potentiometer()
{
	if (!inited)
		error(UNINITIALIZED);
    ADCSRA |= (1 << ADSC); // ADC start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADCH;
}

void encoder_interrupt(int source)
{
	if (!inited)
		error(UNINITIALIZED);
    uint16_t time = TCNT1;
    int16_t dt = time - last_trigger;
	last_trigger = time;
    if (last_trigger_source == source)
        error(BAD_ENCODER_INTERRUPT);
    if (source == 0)
    {
        if (READSPD0 == READSPD1)
            send(ECHO_REPLY, 1);
    }
    else
    {
        if (READSPD0 != READSPD1)
            send(ECHO_REPLY, 2);
    }

    avg_time -= recent_times[time_index];
    avg_time += dt;

    recent_times[time_index] = dt;
    time_index++;
    if (time_index == 10)
        time_index = 0;
    
    last_trigger_source = source;
}

unsigned char get_speed()
{
    return SPEED_CALC_FACTOR / avg_time * 0.1;
}

int16_t get_avg_time()
{
    return avg_time;
}
