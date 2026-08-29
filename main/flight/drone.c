#include "drone.h"
#include "drone_scheduler.h"

// bias
volatile imu_data_t bias;

// layer 1
volatile pid_type_t controller_drone_x, controller_drone_y, controller_drone_z;
volatile pid_type_t controller_drone_roll, controller_drone_pitch, controller_drone_yaw;

// layer 2
volatile pid_type_t controller_drone_velocity_x, controller_drone_velocity_y, controller_drone_rate_yaw;
volatile smc_type_t controller_drone_rate_roll, controller_drone_rate_pitch, controller_drone_velocity_z;

