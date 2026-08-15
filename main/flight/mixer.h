#ifndef _MIXER_H_
#define _MIXER_H_

#include "stdint.h"

typedef struct motorMixer_s {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} motorMixer_t;


typedef struct {
    uint16_t motor_front_r;
    uint16_t motor_rear_r;
    uint16_t motor_front_l;
    uint16_t motor_rear_l;
} motor_output_t;


void write_motor();

/*
    need to tranfer to this function 4 variable
    U1, U2, U3, U4 (Thrust; roll, pitch, yaw (rate))
    4 of that is variable calculate from controller



*/
void mixer_calculate();



#endif