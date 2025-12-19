#ifndef eien65_sensors_h
#define eien65_sensors_h

void init_sensors();
void report_btn(int);
void report_potentiometer();
int read_button(int);
unsigned char read_potentiometer();
void encoder_interrupt(int);
unsigned char get_speed();
int16_t get_avg_time();

#endif