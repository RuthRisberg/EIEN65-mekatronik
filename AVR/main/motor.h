#ifndef eien65_motor_h
#define eien65_motor_h

void init_motor();
void setpwm0(uint8_t);
void setpwm1(uint8_t);
void set_enable(uint8_t); // arg is bool value
void stop_motor();

#endif
