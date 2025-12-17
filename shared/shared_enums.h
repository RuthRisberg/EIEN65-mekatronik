#ifndef eien65_shared_enums_h
#define eien65_shared_enums_h

enum error_msg_t {
    UNINITIALIZED,
    INDEX_OUT_OF_BOUNDS,
    BUFFER_FULL,
    INCORRECT_PARITY,
    INVALID_STATE,
	UNKNOWN_COMMAND
};
char* get_error_msg(enum error_msg_t);

enum msg_avr_to_pc_t {
	INVALID_MSG_AVR,
    ERROR,
    ECHO_REPLY,
	RECEIVED_HEADER,
	RECEIVED_DATA,
	RECEIVED_PARITY
};
char* get_avr_to_pc_header(enum msg_avr_to_pc_t);

enum msg_pc_to_avr_t {
	INVALID_MSG_PC,
    ECHO
};
char* get_pc_to_avr_header(enum msg_pc_to_avr_t);

#endif
