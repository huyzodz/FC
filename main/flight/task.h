#ifndef _TASK_H_
#define _TASK_H_

#include "drone_scheduler.h"
#include "drone.h"
#include "esekf.h"


#define TASK_LENGTH                     7



extern task_t TASK_DRONE [TASK_LENGTH];

volatile extern imu_data_t data_bmi270;
volatile extern imu_data_digital_t data_digital_bmi270;


volatile extern float x_ref;
volatile extern float y_ref;
volatile extern float z_ref;

volatile extern float roll_ref;
volatile extern float pitch_ref;
volatile extern float yaw_ref;






// real time
void task_handle_imu(task_data_t *data);
// use for roll_rate, pitch_rate, yaw_rate and z acc
// this task will also tranfer to U1 U2 U3 U4 and controll motor mixer
void task_handle_rate(task_data_t *data);


// 2 task use for x y
void task_handle_position(task_data_t *data);
// this task will also tranfer acc to roll pitch ref
void task_handle_velocity(task_data_t *data);

// use for roll pitch yaw
void task_handle_attitude(task_data_t *data);

// priority high

// use for fix error for yaw
void task_handle_compass(task_data_t *data);

//fix error for z
void task_handle_barometer(task_data_t *data);
void task_handle_vl53l1x(task_data_t *data);


void task_handle_gps(task_data_t *data);
void task_handle_rasp(task_data_t *data);


#endif