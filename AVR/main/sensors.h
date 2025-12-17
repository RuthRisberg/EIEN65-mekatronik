#ifndef eien65_sensors_h
#define eien65_sensors_h

void init_sensors();
void report_btn(int);
void report_potentiometer();
int read_button(int);
int read_potentiometer();

#endif