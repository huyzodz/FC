#include "pid.h"

#include <stdio.h>
#include <math.h>

static inline float pid_min_max(float val, float min, float max)
{   
    if (val > max)
        return max;
    else if (val < min)
        return min;
    else
        return val;
}


void pid_init(pid_type_t *ret, pid_config_t cfg)
{
    ret->err_previous = 0;
    ret->I = 0;


    ret->setting.kd = cfg.kd;
    ret->setting.ki = cfg.ki;
    ret->setting.kp = cfg.kp;

    ret->setting.max_intergral = cfg.max_intergral;
    ret->setting.min_intergral = cfg.min_intergral;

    ret->setting.max_derivative = cfg.max_derivative;
    ret->setting.min_derivative = cfg.min_derivative;

    ret->setting.max_out = cfg.max_out;
    ret->setting.min_out = cfg.min_out;
}

float pid_calculate(pid_type_t *val, float err, float dt)
{  
    // propotion
    float val_P = val->setting.kp * err;


    // intergral
    // calculate next S in err
    float err_I = err*dt;
    // check
    val->I = pid_min_max(val->I + err_I, val->setting.min_intergral ,val->setting.max_intergral);
    // apply intergral
    float val_I = val->setting.ki * val->I;



    // derivative
    float val_D = val->setting.kd * pid_min_max((err - val->err_previous)/dt, val->setting.min_derivative, val->setting.max_derivative);

    // update err
    val->err_previous = err;


    // return val 
    return pid_min_max(val_P + val_I + val_D, val->setting.min_out, val->setting.max_out);
}