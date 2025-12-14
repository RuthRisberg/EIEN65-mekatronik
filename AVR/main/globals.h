#ifndef eien65_globals_h
#define eien65_globals_h

#include "shared_enums.h"

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

extern char shadow_PORTB;
extern char shadow_PORTC;
extern char shadow_PORTD;

typedef enum state_t state_t; 
enum state_t {
    STATE_BLINKY
};
extern state_t state;

typedef enum error_msg_t error_msg_t;
void error(error_msg_t); // defined in serial (to avoid circular dependencies)

typedef enum transmit_headers_t msg_avr_to_pc_t; 

typedef enum receive_headers_t msg_pc_to_avr_t; 

#endif
