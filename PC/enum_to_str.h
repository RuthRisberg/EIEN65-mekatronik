#ifndef eien65_enum_to_str_h
#define eien65_enum_to_str_h

#include "shared_enums.h"

char* get_error_msg(enum error_msg_t);
char* get_avr_to_pc_header(enum msg_avr_to_pc_t);
char* get_pc_to_avr_header(enum msg_pc_to_avr_t);

#endif
