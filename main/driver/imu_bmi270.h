#ifndef _IMU_BMI270_H_
#define _IMU_BMI270_H_

#include "stdint.h"

/******/
// K = (2000/32768)*pi/180
#define BMI270_GYRO_2_RAD(val)      ((float)val*0.00106526f)

//  (2.0f * 9.80665f / 4096.0f)
#define BMI270_ACC_2_MS2(val)       ((float)val*0.00239467f)

typedef enum {
    BMI270_LOW_POWER_MODE = 0,
    BMI270_NORMAL_POWER_MODE,
    BMI270_HIGH_POWER_MODE
} bmi270_power_mode_t;


typedef struct {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} bmi270_data_t;


typedef struct {
    float gyrox;
    float gyroy;
    float gyroz;

    float accx;
    float accy;
    float accz;
} imu_data_digital_t;

typedef imu_data_digital_t imu_data_t;

typedef struct {
    float p;
    float q;
    float r;
}attitude_t;


int bmi270_init(bmi270_power_mode_t mode);

int bmi270_read(imu_data_digital_t *ret, uint8_t wait_read_done);

void bmi270_calib();





static inline void bmi270_get_body_rate(const imu_data_digital_t *data, attitude_t *ret)
{
    ret->p = BMI270_GYRO_2_RAD(data->gyrox);
    ret->q = BMI270_GYRO_2_RAD(data->gyroy);
    ret->r = BMI270_GYRO_2_RAD(data->gyroz);
}

static inline void bmi270_tranfer_using(const imu_data_digital_t *data, imu_data_t *ret)
{
    ret->accx = BMI270_ACC_2_MS2(data->accx);
    ret->accy = BMI270_ACC_2_MS2(data->accy);
    ret->accz = BMI270_ACC_2_MS2(data->accz);

    ret->gyrox = BMI270_GYRO_2_RAD(data->gyrox);
    ret->gyroy = BMI270_GYRO_2_RAD(data->gyroy);
    ret->gyroz = BMI270_GYRO_2_RAD(data->gyroz);
}



#endif