#ifndef _ESEKF_H_
#define _ESEKF_H_

#include "imu_bmi270.h"
#include "stdint.h"
#include "drone.h"

typedef struct {
    float w;
    float x;
    float y;
    float z;
}quaternion_t;

typedef struct {
    float vx;
    float vy;
    float vz;
} velocity_t;

typedef struct {
    float x;
    float y;
    float z;
} position_t;

typedef struct {
    float roll;
    float pitch;
    float yaw;
} euler_t;

typedef struct {
    float data[3][3];
} matrix_esekf_t;


/*
    only use with data is rad/s
    each variable with k follow is previous
*/



void convert_2_quaternion(quaternion_t *ret, const quaternion_t *qk, const imu_data_t *data, float dt);
void convert_2_velocity(velocity_t *ret, const velocity_t *vk, const quaternion_t *q, const imu_data_t *data, float dt);
void convert_2_position(position_t *ret, const position_t *pk, const velocity_t *vk, float dt);


void quaternion_2_euler(const quaternion_t *q, float *roll, float *pitch, float *yaw);

void esekf_imu_covariance(quaternion_t qua, float dt, imu_data_t imu_data, matrix_esekf_t ret[5][5]);




#endif