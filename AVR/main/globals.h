#ifndef eien65_globals_h
#define eien65_globals_h

char shadow_PORTB = 0;
char shadow_PORTC = 0;
char shadow_PORTD = 0;

typedef enum state_t state_t; 
enum state_t {
    STATE_BLINKY
};
state_t state = STATE_BLINKY;

typedef enum error_msg_t error_msg_t;
enum error_msg_t {
    UNINITIALIZED,
    INDEX_OUT_OF_BOUNDS,
    BUFFER_FULL,
    INCORRECT_PARITY,
    INVALID_STATE
};
void error(error_msg); // defined in serial (to avoid circular dependencies)

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
