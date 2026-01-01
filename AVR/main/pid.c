#include <avr/io.h>
#include "pid.h"
#include "sensors.h"
#include "globals.h"
#include "motor.h"

#define SPEED_KP_SR 4
#define SPEED_KI_SR 4

static uint8_t target_speed = 0; // 0-120
static uint8_t target_pos = 0;
static uint8_t speed_Kp = 16; // >>SPEED_KP_SR
static uint8_t speed_Ki = 1; // >>SPEED_KI_SR
static int16_t speed_integrator = 0; // -120*speed_Ki - 255+120*speed_Ki

void set_target_speed(uint8_t speed)
{
    target_speed = speed;
}
void set_target_position(uint8_t position)
{
    target_pos = position;
}

void speed_control()
{
    uint8_t measured_speed = get_speed(); // 0-120
    int16_t speed_diff = ((int8_t)target_speed) - ((int8_t)measured_speed); // 0-120, 0-120, -120-120

    // integrator, with out-of-bounds checks
    speed_integrator += speed_diff * speed_Ki;

    // find output no overflow
    int16_t pi_controller_out = (speed_integrator>>SPEED_KI_SR); // proportional part
    pi_controller_out += ((((int16_t)target_speed) * ((int16_t)speed_Kp))>>SPEED_KP_SR); // integrated part

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

    // output
    setpwm1((uint8_t)pi_controller_out);
}

void run_pid()
{
    if (state == STATE_SPEED_CONTROL)
        speed_control();
}
