#ifndef _SMC_H_
#define _SMC_H_

#include "imu_bmi270.h"


typedef enum {
    SMC_Z = 0,
    SMC_PITCH,
    SMC_ROLL
}smc_control_type;

typedef struct {
    float w;
    float sigma;
    
    float prev_err;

    float Intergral_sat_phi;
    float MIN_Intergral;
    float MAX_Intergral;

    float desire_previous;


} smc_type_t;



void smc_init(smc_type_t *smc, float w, float sigma, float min_i, float max_i);

/*
    if smc is for z then att = NULL
    d_dot is value from sensor
*/

float smc_calculate(smc_type_t *smc, float dt, float err, float desire, float d_dot, const attitude_t *att, smc_control_type type);


#endif