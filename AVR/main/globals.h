#ifndef eien65_globals_h
#define eien65_globals_h

#include "shared_enums.h"

// check that F_CPU is correct
#ifdef F_CPU
#if(F_CPU != 1000000UL)
#error
#endif
#else
#define F_CPU 1000000UL
#endif

// 5-120 rpm
// 24 interrupts per rotation
// 120-2880 interrupts per minute
// 2-48 interrupts per second
// 2-500 milliseconds per interrupt
// prescaling 64 <=> ~32 cycles per millisecond <=> at least ~64 cycles per interrupt seems good?
// prescaling 64 means (64*2^16)/10^6 = ~4 seconds between each overflow
#define TIMER1_PRESCALING 64

#define BLINKING_LED 0
#define INTERRUPT_LED 1
#define ERROR_LED 2


extern char shadow_PORTB;
extern char shadow_PORTC;
extern char shadow_PORTD;

typedef enum state_t state_t; 
extern state_t state;

typedef enum error_msg_t error_msg_t;
void error(error_msg_t); // defined in serial (to avoid circular dependencies)

typedef enum transmit_headers_t msg_avr_to_pc_t; 

typedef enum receive_headers_t msg_pc_to_avr_t; 

#endif
