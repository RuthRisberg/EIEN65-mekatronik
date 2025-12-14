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
char* get_error_msg(enum error_msg_t error)
{
    switch (error)
    {
        case UNINITIALIZED: return "UNINITIALIZED";
        case INDEX_OUT_OF_BOUNDS: return "INDEX_OUT_OF_BOUNDS";
        case BUFFER_FULL: return "BUFFER_FULL";
        case INCORRECT_PARITY: return "INCORRECT_PARITY";
        case INVALID_STATE: return "INVALID_STATE";
        case UNKNOWN_COMMAND: return "UNKNOWN_COMMAND";
        default: return "unknown error message";
    }
}

enum msg_avr_to_pc_t {
    ERROR,
    ECHO_REPLY
};
char* get_avr_to_pc_header(enum msg_avr_to_pc_t header)
{
    switch (header)
    {
        case ERROR: return "ERROR";
        case ECHO_REPLY: return "ECHO_REPLY";
        default: return "unknown header";
    }
}

enum msg_pc_to_avr_t {
    ECHO
};
char* get_pc_to_avr_header(enum msg_pc_to_avr_t header)
{
    switch (header)
    {
        case ECHO: return "ECHO";
        default: return "unknown header";
    }
}

#endif
