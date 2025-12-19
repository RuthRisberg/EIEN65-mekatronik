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
		case SPEED: "SPEED";
		case AVG_TIME_H: "AVG_TIME_H";
		case AVG_TIME_L: "AVG_TIME_L";
		default: return "unknown header";
	}
}

char* get_pc_to_avr_header(enum msg_pc_to_avr_t header)
{
	switch (header)
	{
		case INVALID_MSG_PC: "INVALID_MSG_PC";
		case ECHO: "ECHO";
		case SET_STATE_NONE: "SET_STATE_NONE";
		case GET_STATE: "GET_STATE";
		case START_BLINK_LED: "START_BLINK_LED";
		case STOP_BLINK_LED: "STOP_BLINK_LED";
		case START_REPORT_BTNS: "START_REPORT_BTNS";
		case STOP_REPORT_BTNS: "STOP_REPORT_BTNS";
		case START_REPORT_POTENTIOMETER: "START_REPORT_POTENTIOMETER";
		case STOP_REPORT_POTENTIOMETER: "STOP_REPORT_POTENTIOMETER";
		case READ_BTNS: "READ_BTNS";
		case READ_POTENTIOMETER: "READ_POTENTIOMETER";
		case TURN_ON_LED: "TURN_ON_LED";
		case TURN_OFF_LED: "TURN_OFF_LED";
		case SET_PWM1: "SET_PWM1";
		case SET_PWM2: "SET_PWM2";
		case GET_SPEED: "GET_SPEED";
		case GET_AVG_TIME: "GET_AVG_TIME";
		default: return "unknown header";
	}
}
