#include "enum_to_str.h"

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
		case BAD_ENCODER_INTERRUPT: return "BAD_ENCODER_INTERRUPT";
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
		case RECEIVED_HEADER: return "RECEIVED_HEADER";
		case RECEIVED_DATA: return "RECEIVED_DATA";
		case RECEIVED_PARITY: return "RECEIVED_PARITY";
		case BTN_STATUS: return "BTN_STATUS";
		case POTENTIOMETER_STATUS: return "POTENTIOMETER_STATUS";
		case CURRENT_STATE: return "CURRENT_STATE";
		case SPEED: return "SPEED";
		case AVG_TIME_H: return "AVG_TIME_H";
		case AVG_TIME_L: return "AVG_TIME_L";
		default: return "unknown header";
	}
}

char* get_pc_to_avr_header(enum msg_pc_to_avr_t header)
{
	switch (header)
	{
		case INVALID_MSG_PC: return "INVALID_MSG_PC";
		case ECHO: return "ECHO";
		case SET_STATE_NONE: return "SET_STATE_NONE";
		case GET_STATE: return "GET_STATE";
		case START_BLINK_LED: return "START_BLINK_LED";
		case STOP_BLINK_LED: return "STOP_BLINK_LED";
		case START_REPORT_BTNS: return "START_REPORT_BTNS";
		case STOP_REPORT_BTNS: return "STOP_REPORT_BTNS";
		case START_REPORT_POTENTIOMETER: return "START_REPORT_POTENTIOMETER";
		case STOP_REPORT_POTENTIOMETER: return "STOP_REPORT_POTENTIOMETER";
		case READ_BTNS: return "READ_BTNS";
		case READ_POTENTIOMETER: return "READ_POTENTIOMETER";
		case TURN_ON_LED: return "TURN_ON_LED";
		case TURN_OFF_LED: return "TURN_OFF_LED";
		case SET_PWM1: return "SET_PWM1";
		case SET_PWM2: return "SET_PWM2";
		case GET_SPEED: return "GET_SPEED";
		case GET_AVG_TIME: return "GET_AVG_TIME";
		default: return "unknown header";
	}
}
