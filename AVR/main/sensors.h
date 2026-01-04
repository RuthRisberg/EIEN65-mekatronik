#ifndef eien65_sensors_h
#define eien65_sensors_h


void init_sensors();
void report_btn(int);
void report_potentiometer();
int read_button(int);
unsigned char read_potentiometer();
void encoder_interrupt_0();
void encoder_interrupt_1();
int16_t get_speed(); // needs to be signed to handle backwards speed and 120 vs 128 is a bit too close to overflow
int16_t get_avg_time();
void report_interrupt_stats();
void report_recent_times();

#endif