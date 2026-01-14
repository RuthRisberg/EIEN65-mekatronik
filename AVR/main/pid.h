#ifndef eien65_pid_h
#define eie65_pid_h

void set_target_speed(uint8_t speed);
void set_target_position(uint8_t position);
void set_speed_Kp(uint8_t new_Kp);
void set_speed_Ki(uint8_t new_Ki);
void toggle_reporting_integrator();
void toggle_reporting_PI_output();
void flip_directions();
void run_pid();

#endif
