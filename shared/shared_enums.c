#include "shared_enums.h"

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

char* get_avr_to_pc_header(enum msg_avr_to_pc_t header)
{
	switch (header)
	{
		case INVALID_MSG_AVR: return "INVALID_MSG_AVR";
		case ERROR: return "ERROR";
		case ECHO_REPLY: return "ECHO_REPLY";
		default: return "unknown header";
	}
}

char* get_pc_to_avr_header(enum msg_pc_to_avr_t header)
{
	switch (header)
	{
		case INVALID_MSG_PC: return "INVALID_MSG_PC";
		case ECHO: return "ECHO";
		default: return "unknown header";
	}
}
