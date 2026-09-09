#ifndef _SMC_H_
#define _SMC_H_

#include "imu_bmi270.h"

#include "define_sim.h"


typedef enum {
    SMC_Z = 0,
    SMC_PITCH,
    SMC_ROLL
}smc_control_type;


#ifdef SIMULATION_ON

typedef struct {
    float err;
    float d_dot;
    float V_desire_dot;
} debug_smc_t;

#endif

typedef struct {
    // test variable
#ifdef SIMULATION_ON
    debug_smc_t debug;
#endif

    float w;
    float sigma;
    
    float prev_err;

    float Intergral_sat_phi;
    float MIN_Intergral;
    float MAX_Intergral;

    float desire_previous;


} smc_type_t;


// layer 2
volatile extern smc_type_t controller_drone_rate_roll, controller_drone_rate_pitch, controller_drone_velocity_z;


void smc_init(smc_type_t *smc, float w, float sigma, float min_i, float max_i);

/*
    if smc is for z then att = NULL
    d_dot is value from sensor
*/

float smc_calculate(smc_type_t *smc, float dt, float err, float desire, float d_dot, const attitude_t *att, smc_control_type type);


#endif