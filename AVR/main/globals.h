#ifndef eien65_globals_h
#define eien65_globals_h

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
enum error_msg_t {
    UNINITIALIZED,
    INDEX_OUT_OF_BOUNDS,
    BUFFER_FULL,
    INCORRECT_PARITY,
    INVALID_STATE,
	UNKNOWN_COMMAND
};
void error(error_msg_t); // defined in serial (to avoid circular dependencies)

typedef enum transmit_headers_t transmit_headers_t; 
enum transmit_headers_t {
    ERROR,
    ECHO_REPLY
};

typedef enum receive_headers_t receive_headers_t; 
enum receive_headers_t {
    ECHO
};

#endif
