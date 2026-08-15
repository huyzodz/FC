#ifndef _PID_H_
#define _PID_H_


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

typedef struct {
    // intergral
    float I;

    // err
    float err_previous;

    pid_config_t setting;
} pid_type_t;


void pid_init(pid_type_t *ret, pid_config_t cfg);
float pid_calculate(pid_type_t *val, float err, float dt);


#endif