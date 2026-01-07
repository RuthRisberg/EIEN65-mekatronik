#ifndef eien65_globals_h
#define eien65_globals_h

// check that F_CPU is correct
#ifdef F_CPU
#if(F_CPU != 1000000UL)
#error
#endif
#else
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include "shared_enums.h"

// 5-120 rpm
// 24 interrupts per rotation
// 120-2880 interrupts per minute
// 2-48 interrupts per second
// 20-500 milliseconds per interrupt
// prescaling 256 <=> ~4 slowcycles per millisecond <=> at least ~78 slowcycles per interrupt seems good
// prescaling 256 means (256*2^16)/10^6 = ~16 seconds between each overflow
// up to 5*10^5/256 = ~1950 slow cycles per interrupt
// means avg can store the sum of at most 2^15/1950 >~ 16 interrupts => use bigger variable for avg to be safe
#define TIMER1_PRESCALING 256

#define BLINKING_LED 0
#define INTERRUPT_LED 1
#define ERROR_LED 2


extern uint8_t shadow_PORTB;
extern uint8_t shadow_PORTC;
extern uint8_t shadow_PORTD;

typedef enum state_t state_t; 
extern state_t state;

typedef enum error_msg_t error_msg_t;
void error(error_msg_t); // defined in serial (to avoid circular dependencies)

typedef enum transmit_headers_t msg_avr_to_pc_t; 

typedef enum receive_headers_t msg_pc_to_avr_t; 

#endif
