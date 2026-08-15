#ifndef _DRONE_H_
#define _DRONE_H_

#include "pid.h"
#include "smc.h"

#define DRONE_Ax                        0.25f
#define DRONE_Ay                        0.25f
#define DRONE_Az                        0.25f
#define DRONE_Ix                        4.856e-3f
#define DRONE_Iy                        4.856e-3f
#define DRONE_Iz                        8.856e-3f
#define DRONE_Jr                        2.93e-3f
#define DRONE_g                         9.81f

#define DRONE_Omr                       0.0f
    
#define DRONE_m                         1.0f
#define DRONE_l                         0.18f



// layer 1
volatile extern pid_type_t drone_x, drone_y, drone_z;
volatile extern pid_type_t drone_roll, drone_pitch, drone_yaw;

// layer 2
volatile extern pid_type_t drone_rate_x, drone_rate_y, drone_rate_yaw;
volatile extern smc_type_t drone_rate_roll, drone_rate_pitch, drone_rate_z;

#endif