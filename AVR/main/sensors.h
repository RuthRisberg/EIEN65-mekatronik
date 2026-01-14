#ifndef eien65_sensors_h
#define eien65_sensors_h


void init_sensors();
void report_btn(uint8_t);
void report_potentiometer();
uint8_t read_button(uint8_t);
uint8_t read_potentiometer();
uint8_t read_position();
void encoder_interrupt_0();
void encoder_interrupt_1();
int16_t get_speed(); // needs to be signed to handle backwards speed and 120 vs 128 is a bit too close to overflow
int16_t get_avg_time();
void report_interrupt_stats();
void report_recent_times();

#endif
