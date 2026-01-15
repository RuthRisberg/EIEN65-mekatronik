#include "globals.h"
#include <avr/io.h>
#include "sensors.h"
#include "shared_enums.h"
#include "serial.h"
#include "leds.h"

#define BTN0 PD4
#define BTN1 PB6
#define BTN2 PC1
#define POT ((uint8_t) 3) // PC3 ADC3
#define POS ((uint8_t) 0) // PC0 ADC0
#define SPD0 PD7
#define READSPD0 ((PIND & (1 << SPD0)) >> SPD0)
#define SPD1 PB0
#define READSPD1 ((PINB & (1 << SPD1)) >> SPD1)

// 1/N_TIMES_SAVED = 1-(MULT_AVG_FACTOR/(2^MULT_AVG_SHIFT)) should be true when doing mult avg
#define N_TIMES_SAVED 32 // at most 256, index is saved in uint8_t
#define MULT_AVG_FACTOR 15
#define MULT_AVG_SHIFT 4
#define ADD_AVG
//#define MULT_AVG

// #define TRIGGERS_PER_ROTATION 24*4 // 24 pulses, 4 triggers per pulse (?)
// #define CLOCK_CYCLES_PER_MINUTE (F_CPU*60)
// #define SPEED_CALC_FACTOR (CLOCK_CYCLES_PER_MINUTE * N_TIMES_SAVED / TIMER1_PRESCALING / TRIGGERS_PER_ROTATION)
#define SPEED_CALC_FACTOR ((int32_t) 78125) // 10**6 * 60 * 32 / 256 / (24*4)
// 1s * F_CPU / TIMER1_PRESCALING
#define TIME_UNTIL_STANDING_STILL 7813 // 2 * 10**6 / 256

#define KEEP_RECENT_TIMES

static int inited = 0;

// for speed calculations
static uint16_t last_trigger = 0;
static int16_t recent_times[N_TIMES_SAVED];
static int32_t avg_time = 0; // 16 bit is nearly too small for 5rpm, prescaling=256, N_TIMES_SAVED>4
static uint8_t time_index = 0;
static uint8_t last_trigger_source = 0;
static uint8_t standing_still = 1;

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

    // analog inputs (potentiometer and position)
    ADMUX |= (1 << ADLAR); // left adjust result (most significant 8 bits in ADCH)
    ADCSRA |= (1 << ADEN); // ADC enable

    // digital encoder
    // set pins to input with pull-up
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
    if (TIMER1_PRESCALING == 1)
        TCCR1B = 1;
    else if (TIMER1_PRESCALING == 8)
        TCCR1B = 2;
    else if (TIMER1_PRESCALING == 64)
        TCCR1B = 3;
    else if (TIMER1_PRESCALING == 256)
        TCCR1B = 4;
    else if (TIMER1_PRESCALING == 1024)
        TCCR1B = 5;
    else
        error(UNHANDLED_CASE);

    for (int i = 0; i < N_TIMES_SAVED; i++)
        recent_times[i] = 0;
    
    inited = 1;
}

void report_btn(uint8_t btn)
{
    send(BTN_STATUS, btn*10 + read_button(btn)); // *10 instead of a left shift to be human readable
}

void report_potentiometer()
{
    send(POTENTIOMETER_STATUS, read_potentiometer());
}

uint8_t read_button(uint8_t btn)
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

uint8_t read_potentiometer()
{
    if (!inited)
        error(UNINITIALIZED);
    ADMUX &= 0xf0; // clear pin selection
    ADMUX |= POT; // set the correct pin to read from
    ADCSRA |= (1 << ADSC); // ADC start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADCH;
}

uint8_t read_position()
{
    if (!inited)
        error(UNINITIALIZED);
    ADMUX &= 0xf0; // clear pin selection
    ADMUX |= POS; // set the correct pin to read from
    ADCSRA |= (1 << ADSC); // ADC start conversion
    while (ADCSRA & (1 << ADSC)); // wait for conversion to finish
    return ADCH;
}

static uint8_t good_interrupts = 0;
static uint8_t bad_interrupts = 0;
void encoder_interrupt_0()
{
    // not checking initialized because this will run *very* frequently when the motor is spinning fast
    uint16_t time = TCNT1;
    standing_still = 0;
    // recent_times[time_index] = time;
    // recent_times[time_index] = READSPD0 | READSPD1;
    // time_index++;
    // if (time_index == N_TIMES_SAVED)
    //     time_index = 0;
    // toggle_led(4);
    // return;
    uint16_t temp = time - last_trigger; // need this subtraction unsigned to handle overflow
    int16_t dt = temp; // might flip sign later so needs to be signed
    last_trigger = time;

    if (last_trigger_source == 0)
    {
        // bad_interrupts++;
        // turn_on_led(4);
        // turn_off_led(5);
    //     error(BAD_ENCODER_INTERRUPT);
        return;
    }
    // else
    // {
    //     turn_on_led(5);
    //     turn_off_led(4);
    //     good_interrupts++;
    // }
    last_trigger_source = 0;

    if (READSPD0 == READSPD1)
        dt = -dt;

#if defined(ADD_AVG)
    avg_time -= recent_times[time_index];
#elif defined(MULT_AVG)
    avg_time *= MULT_AVG_FACTOR;
    avg_time >>= MULT_AVG_SHIFT;
#endif
    avg_time += dt;
    if (avg_time < -100000000 || avg_time > 100000000)
        error(OVERFLOW);

#ifdef KEEP_RECENT_TIMES
    recent_times[time_index] = dt;
    // recent_times[time_index] = temp;
    // recent_times[time_index] = (READSPD0>>6) | READSPD1;
    // if (READSPD0 != READSPD1)
    //     recent_times[time_index] += 10;
    // recent_times[time_index] += 1000;
    time_index++;
    if (time_index == N_TIMES_SAVED)
        time_index = 0;
#endif
}

void encoder_interrupt_1()
{
    // not checking initialized because this will run *very* frequently when the motor is spinning fast
    uint16_t time = TCNT1;
    standing_still = 0;
    // recent_times[time_index] = time;
    // recent_times[time_index] = READSPD0 | READSPD1;
    // time_index++;
    // if (time_index == N_TIMES_SAVED)
    //     time_index = 0;
    // toggle_led(5);
    // return;
    uint16_t temp = time - last_trigger; // need this subtraction unsigned to handle overflow
    int16_t dt = temp; // might flip sign later so needs to be signed
    last_trigger = time;

    if (last_trigger_source == 1)
    {
        // bad_interrupts++;
        // turn_on_led(4);
        // turn_off_led(5);
    //     error(BAD_ENCODER_INTERRUPT);
        return;
    }
    // else
    // {
    //     turn_on_led(5);
    //     turn_off_led(4);
    //     good_interrupts++;
    // }
    last_trigger_source = 1;

    if (READSPD0 != READSPD1)
        dt = -dt;

#if defined(ADD_AVG)
    avg_time -= recent_times[time_index];
#elif defined(MULT_AVG)
    avg_time *= MULT_AVG_FACTOR;
    avg_time >>= MULT_AVG_SHIFT;
#endif
    avg_time += dt;
    if (avg_time < -100000000 || avg_time > 100000000)
        error(OVERFLOW);

#ifdef KEEP_RECENT_TIMES
    recent_times[time_index] = dt;
    // recent_times[time_index] = temp;
    // recent_times[time_index] = (READSPD0>>6) | READSPD1;
    // if (READSPD0 != READSPD1)
    //     recent_times[time_index] += 10;
    // recent_times[time_index] += 1100;
    time_index++;
    if (time_index == N_TIMES_SAVED)
        time_index = 0;
#endif
}

void report_recent_times()
{
#ifdef KEEP_RECENT_TIMES
    for (int i = 0; i < N_TIMES_SAVED; i++)
    {
        // send(WHICH_INTERRUPT, i);
        send(INTERRUPT_TIME_H, recent_times[i]>>8);
        send(INTERRUPT_TIME_L, recent_times[i]);
    }
#endif
}

void report_interrupt_stats()
{
    send(GOOD_INTERRUPTS, good_interrupts);
    send(BAD_INTERRUPTS, bad_interrupts);
    good_interrupts = 0;
    bad_interrupts = 0;
}

int16_t get_speed()
{
    uint16_t time_since_last = TCNT1 - last_trigger;
    if (time_since_last > TIME_UNTIL_STANDING_STILL)
        standing_still = 1;
    if (standing_still)
        return 0;
    return SPEED_CALC_FACTOR / avg_time;
}

int16_t get_avg_time()
{
    return avg_time / N_TIMES_SAVED;
}
