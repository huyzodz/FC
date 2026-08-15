#include "smc.h"
#include <math.h>
#include "drone.h"



static inline float sat(float val)
{
    return (val/(((val < 0) ? val*(-1) : val)+0.1f));
}

static inline float Max_Min_Intergral_SMC(float val, float min, float max)
{
    if (val > max)
        return max;
    else if (val < min)
        return min;
    else
        return val;
}


void smc_init(smc_type_t *smc, float w, float sigma, float min_i, float max_i)
{
    *smc = (smc_type_t) {
        .desire_previous = 0,
        .Intergral_sat_phi = 0,
        .prev_err = 0,
        .sigma = sigma,
        .w = w,
        .MIN_Intergral = min_i,
        .MAX_Intergral = max_i
    };
}

float smc_calculate(smc_type_t *smc, float dt, float err, float desire, float d_dot, const attitude_t *att, smc_control_type type)
{
    float sat_phi;
    // base on formula
    float phi = err;
    float V_desire_dot = (desire - smc->desire_previous)/dt;
    

    //calculate phi and sat_phi
    sat_phi = sat(phi);

    // calculate intergral
    smc->Intergral_sat_phi = Max_Min_Intergral_SMC(
        smc->Intergral_sat_phi + sat_phi*dt,
        smc->MIN_Intergral,
        smc->MAX_Intergral
    );

    // update previous
    smc->desire_previous = desire;

    //return SMC control signal
    if (type == SMC_Z)
        return (DRONE_m*((-DRONE_Az/DRONE_m)*d_dot + DRONE_g - V_desire_dot) - smc->sigma*sqrtf(fabsf(phi))*sat_phi - smc->w*smc->Intergral_sat_phi);
    else if (type == SMC_PITCH)
        // for pitch
        return ((DRONE_Iy/DRONE_l)*(-att->p*att->r*(DRONE_Iz - DRONE_Ix)/DRONE_Iy - att->p*(DRONE_Jr/DRONE_Iy)*DRONE_Omr + V_desire_dot) - smc->sigma*sqrtf(fabsf(phi))*sat_phi - smc->w*smc->Intergral_sat_phi);
    else
        // for roll
        return ((DRONE_Ix/DRONE_l)*(-att->q*att->r*(DRONE_Iy - DRONE_Iz)/DRONE_Ix - att->q*(DRONE_Jr/DRONE_Ix)*DRONE_Omr + V_desire_dot) - smc->sigma*sqrtf(fabsf(phi))*sat_phi - smc->w*smc->Intergral_sat_phi);

}