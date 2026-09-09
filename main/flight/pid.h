#ifndef _PID_H_
#define _PID_H_


#include "define_sim.h"

typedef struct {

    // limit for intergral
    float min_intergral, max_intergral;

    // limit for derivative
    float min_derivative, max_derivative;

    //limit for output
    float min_out, max_out;

    // save kp ki kd
    float kp, ki, kd;
} pid_config_t;


#ifdef SIMULATION_ON

typedef struct {
    float output;
} debug_pid_t;

#endif

typedef struct {

#ifdef SIMULATION_ON

    debug_pid_t debug;

#endif
    // intergral
    float I;

    // err
    float err_previous;

    pid_config_t setting;
} pid_type_t;



// layer 1
volatile extern pid_type_t controller_drone_x, controller_drone_y, controller_drone_z;
volatile extern pid_type_t controller_drone_roll, controller_drone_pitch, controller_drone_yaw;

// layer 2
volatile extern pid_type_t controller_drone_velocity_x, controller_drone_velocity_y, controller_drone_rate_yaw;


void pid_init(pid_type_t *ret, pid_config_t cfg);
float pid_calculate(pid_type_t *val, float err, float dt);


#endif