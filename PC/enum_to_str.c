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
        case UNHANDLED_CASE: return "UNHANDLED_CASE";
        case OVERFLOW: return "OVERFLOW";
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
        case SPEED_H: return "SPEED_H";
        case SPEED_L: return "SPEED_L";
        case AVG_TIME_H: return "AVG_TIME_H";
        case AVG_TIME_L: return "AVG_TIME_L";
        case WAKE_UP_TIME_H: return "WAKE_UP_TIME_H";
        case WAKE_UP_TIME_L: return "WAKE_UP_TIME_L";
        case INTERRUPT_TIME_H: return "INTERRUPT_TIME_H";
        case INTERRUPT_TIME_L: return "INTERRUPT_TIME_L";
        case WHICH_INTERRUPT: return "WHICH_INTERRUPT";
        case GOOD_INTERRUPTS: return "GOOD_INTERRUPTS";
        case BAD_INTERRUPTS: return "BAD_INTERRUPTS";
        case INTEGRATOR_H: return "INTEGRATOR_H";
        case INTEGRATOR_L: return "INTEGRATOR_L";
        case PI_OUTPUT: return "PI_OUTPUT";
        case TARGET_SPEED: return "TARGET_SPEED";
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
        case SET_PWM0: return "SET_PWM0";
        case SET_PWM1: return "SET_PWM1";
        case GET_SPEED: return "GET_SPEED";
        case GET_AVG_TIME: return "GET_AVG_TIME";
        case GET_INTERRUPT_STATS: return "GET_INTERRUPT_STATS";
        case GET_RECENT_TIMES: return "GET_RECENT_TIMES";
        case SET_SPEED: return "SET_SPEED";
        case SET_SPEED_KP: return "SET_SPEED_KP";
        case SET_SPEED_KI: return "SET_SPEED_KI";
        case TOGGLE_REPORT_INTEGRATOR: return "TOGGLE_REPORT_INTEGRATOR";
        case TOGGLE_REPORT_PI_OUTPUT: return "TOGGLE_REPORT_PI_OUTPUT";
        case TOGGLE_REPORT_SPEED: return "TOGGLE_REPORT_SPEED";
        case FLIP_POS_CONTROL_DIRECTION: return "FLIP_POS_CONTROL_DIRECTION";
        case TOGGLE_REPORT_TARGET_SPEED: return "TOGGLE_REPORT_TARGET_SPEED";
        case TOGGLE_USE_POTENTIOMETER: return "TOGGLE_USE_POTENTIOMETER";
        default: return "unknown header";
    }
}
