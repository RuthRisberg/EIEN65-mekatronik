#include "globals.h"
#include <avr/io.h>
#include "pid.h"
#include "sensors.h"
#include "motor.h"
#include "serial.h"
#include <stdlib.h>

#define SPEED_KP_SR 4
#define SPEED_KI_SR 4

static uint8_t target_speed = 0; // 0-120
static uint8_t target_pos = 0;
static uint8_t speed_Kp = 18; // >>SPEED_KP_SR
static uint8_t speed_Ki = 4; // >>SPEED_KI_SR
static int16_t speed_integrator = 0; // -120*speed_Ki - 255+120*speed_Ki
static uint8_t reporting_integrator = 0;
static uint8_t reporting_output = 0;
static uint8_t reporting_target_speed = 0;
static uint8_t using_potentiometer = 1;
static uint8_t flip_direction = 0;

void set_target_speed(uint8_t speed)
{
    target_speed = speed;
}
void set_target_position(uint8_t position)
{
    target_pos = position;
}
void set_speed_Kp(uint8_t new_Kp)
{
    speed_Kp = new_Kp;
}
void set_speed_Ki(uint8_t new_Ki)
{
    speed_Ki = new_Ki;
}
void toggle_reporting_integrator()
{
    reporting_integrator = 1-reporting_integrator;
}
void toggle_reporting_PI_output()
{
    reporting_output = 1-reporting_output;
}
void toggle_reporting_target_speed()
{
    reporting_target_speed = 1-reporting_target_speed;
}
void toggle_use_potentiometer()
{
    using_potentiometer = 1-using_potentiometer;
}

static void speed_control()
{
    uint8_t measured_speed = abs(get_speed()); // 0-120 (or maybe slightly more)
    uint8_t adjusted_target_speed = target_speed;
    if (using_potentiometer)
    {
        int8_t adjustment = (((int16_t)read_potentiometer())-128)*10/128;
        adjusted_target_speed += adjustment;
        if (adjusted_target_speed < 5)
            adjusted_target_speed = 5;
        if (adjusted_target_speed > 120)
            adjusted_target_speed = 120;
    }
    int8_t speed_diff = ((int8_t)adjusted_target_speed) - ((int8_t)measured_speed); // 0-120, 0-120, -120-120
    if (reporting_target_speed)
        send(TARGET_SPEED, adjusted_target_speed);

    // integrator, will not overflow due to anti-windup
    speed_integrator += ((int16_t)speed_diff) * ((int16_t)speed_Ki);

    // find output no overflow
    int16_t pi_controller_out = (speed_integrator>>SPEED_KI_SR); // integrated part
    pi_controller_out += ((((int16_t)adjusted_target_speed) * ((int16_t)speed_Kp))>>SPEED_KP_SR); // proportional part

    // anti-windup
    if (pi_controller_out < 0)
    {
        speed_integrator -= pi_controller_out<<SPEED_KI_SR;
        pi_controller_out = 0;
    }
    else if (pi_controller_out > 255)
    {
        speed_integrator -= (pi_controller_out-255)<<SPEED_KI_SR;
        pi_controller_out = 255;
    }
    if (reporting_integrator)
    {
        send(INTEGRATOR_H, speed_integrator>>8);
        send(INTEGRATOR_L, speed_integrator);
    }

    // output
    if (reporting_output)
    {
        send(PI_OUTPUT, 255-pi_controller_out);
    }
    setpwm0(255-pi_controller_out);
}

void flip_directions()
{
    flip_direction = 1-flip_direction;
}

static void position_control()
{
    uint8_t measured_pos = read_position(); // idk range
    int16_t pos_diff = ((int16_t)target_pos) - ((int16_t)measured_pos);
    if (pos_diff < 5 && pos_diff > -5)
    {
        set_enable(0);
        return;
    }

    int16_t pos_control_out = pos_diff;

    if (flip_direction)
        pos_control_out = -pos_control_out;

    if (pos_control_out > 0)
    {
        setpwm0(pos_control_out);
        setpwm1(0);
    }
    else
    {
        setpwm0(0);
        setpwm1(-pos_control_out);
    }
    set_enable(1);
}

void run_pid()
{
    if (state == STATE_SPEED_CONTROL)
        speed_control();
}
