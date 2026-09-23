#include "task.h"
#include "mixer.h"
#include "timer.h"
#include "math.h"
#include "esekf.h"
#include "barometer_dps310.h"
#include "gps_be880.h"
#include "pid.h"
#include "smc.h"



#include <stddef.h>



#ifdef SIMULATION_ON

#include "simulate.h"

volatile simulate_data_rx_t data_sim;

#endif


// 15 der
#define MAX_RAD_ROLL_REF                0.261799f
#define MIN_RAD_ROLL_REF               -0.261799f

#define MAX_RAD_PITCH_REF               0.261799f
#define MIN_RAD_PITCH_REF              -0.261799f


//#define RANGE_RAD_ROLL(val)         (((val) > MAX_RAD_ROLL_REF) ? MAX_RAD_ROLL_REF : ((val) < MIN_RAD_ROLL_REF) ? MIN_RAD_ROLL_REF : (val))
//#define RANGE_RAD_PITCH(val)        (((val) > MAX_RAD_PITCH_REF) ? MAX_RAD_PITCH_REF : ((val) < MIN_RAD_PITCH_REF) ? MIN_RAD_PITCH_REF : (val))


static inline float clampf(float val, float min, float max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}


// init some info task here, then init with function in file init
task_t TASK_DRONE [TASK_LENGTH] = 
{
    {
        .name_task = "imu_handle",
        .freq = 1000,
        .last_start_time = 0,
        .init_time_run = 2000,
        .last_stop_time = 0,
        .priority = TASK_REAL_TIME,
        .task = &task_handle_imu
    },
    {
        .name_task = "handle_rate",
        .freq = 1000,
        .last_start_time = 0,
        .init_time_run = 2700,
        .last_stop_time = 0,
        .priority = TASK_REAL_TIME,
        .task = &task_handle_rate
    },
    {
        .name_task = "handle_attitude",
        .freq = 300,
        .last_start_time = 0,
        .init_time_run = 2500,
        .last_stop_time = 0,
        .priority = 0,
        .task = &task_handle_attitude
    },
    {
        .name_task = "handle_velocity",
        .freq = 200,
        .last_start_time = 0,
        .init_time_run = 1700,
        .last_stop_time = 0,
        .priority = 1,
        .task = &task_handle_velocity
    },
    {
        .name_task = "handle_position",
        .freq = 50,
        .last_start_time = 0,
        .init_time_run = 1400,
        .last_stop_time = 0,
        .priority = 1,
        .task = &task_handle_position
    },
    {
        .name_task = "handle_compass",
        .freq = 200,
        .last_start_time = 0,
        .init_time_run = 3300,
        .last_stop_time = 0,
        .priority = 2,
        .task = &task_handle_compass
    },
    {
        .name_task = "handle_barometer",
        .freq = 50,
        .last_start_time = 0,
        .init_time_run = 3000,
        .last_stop_time = 0,
        .priority = 2,
        .task = &task_handle_barometer
    },
};





/*
    flobal variable use in this file
*/

// debug only
volatile imu_data_t data_bmi270;
volatile imu_data_digital_t data_digital_bmi270;

//#ifdef SIMULATION_ON




//#endif

// use in esekf
// block is 3x3
// matrix is 15x15
static matrix_esekf_t P[5][5];


// varible ref for layer 1

volatile float roll_ref = 0.0f;
volatile float pitch_ref = 0.0f;
volatile float yaw_ref = 0.0f;

volatile float x_ref = 0.0f;
volatile float y_ref = 0.0f;
volatile float z_ref = 0.0f;


// variable ref for layer 2
static float x_velocity_ref = 0.0f;
static float y_velocity_ref = 0.0f;
static float z_velocity_ref = 0.0f;

static float roll_rate_ref = 0.0f;
static float pitch_rate_ref = 0.0f;
static float yaw_rate_ref = 0.0f;



//      ******* TEST ***** ONLY
//  *********************** MUST DELETE WHEN DONE ****************************



/*

    real time task

*/
void task_handle_imu(task_data_t *data)
{
    float dt = data->dt;
    

#ifdef SIMULATION_ON
    // read data from gazebo here
    while (simulate_getData(&data_sim) != 0);

    data_bmi270.accx = data_sim.accx - bias.accx;
    data_bmi270.accy = data_sim.accy - bias.accy;
    data_bmi270.accz = data_sim.accz - bias.accz;
    data_bmi270.gyrox = data_sim.gyrox - bias.gyrox;
    data_bmi270.gyroy = data_sim.gyroy - bias.gyroy;
    data_bmi270.gyroz = data_sim.gyroz - bias.gyroz;

    drone_attitute.p = data_bmi270.gyrox;
    drone_attitute.q = data_bmi270.gyroy;
    drone_attitute.r = data_bmi270.gyroz;
#else
    
    //imu_data_t data_bmi270;
    //imu_data_digital_t data_digital_bmi270;
    // read raw data rad/s
    bmi270_read(&data_digital_bmi270, 1);
    // tranfer to rad in gryo
    //bmi270_get_body_rate(&data_digital_bmi270, &drone_attitute);
    bmi270_tranfer_using(&data_digital_bmi270, &data_bmi270);

    // tranfer to attitude
    drone_attitute.p = data_bmi270.gyrox;
    drone_attitute.q = data_bmi270.gyroy;
    drone_attitute.r = data_bmi270.gyroz;

#endif
    quaternion_t temp_q = drone_quaternion;

    // 3 function below is prediction from bmi270
    // norminal state
    // get quaternion
    convert_2_quaternion(&drone_quaternion, &temp_q, &data_bmi270, dt);

    // get velocity
    convert_2_velocity(&drone_velocity, &drone_velocity, &drone_quaternion, &data_bmi270, dt);

    // get position
    convert_2_position(&drone_position, &drone_position, &drone_velocity, dt);

    

    // covariance
    esekf_imu_covariance(drone_quaternion, dt, data_bmi270, P);
}


// remember to handle yaw to recalculare x y
void task_handle_position(task_data_t *data)
{   
    float dt = data->dt;
    // ref of position can be from rasp
    // position from drone position, this variable will predict and fix wrone base on gps and camera
    float err_x = x_ref - drone_position.x;
    float err_y = y_ref - drone_position.y;
    float err_z = z_ref - drone_position.z;



    // handle pid for x y z
    // position for x y z
    x_velocity_ref = pid_calculate(&controller_drone_x, err_x, dt);
    y_velocity_ref = pid_calculate(&controller_drone_y, err_y, dt);
    z_velocity_ref = pid_calculate(&controller_drone_z, err_z, dt);
}

void task_handle_velocity(task_data_t *data)
{
    float dt = data->dt;
    float accelerate_x;
    float accelerate_y;
    float roll, pitch, yaw;
    // this variable yaw need to read careful
    // use data from esksf which fix by gps, camera and imu
    float cy;
    float sy;
    float ax_body, ay_body;

    // handle x y
    float err_velocity_x = x_velocity_ref - drone_velocity.vx;
    float err_velocity_y = y_velocity_ref - drone_velocity.vy;
    accelerate_x = pid_calculate(&controller_drone_velocity_x, err_velocity_x, dt);
    accelerate_y = pid_calculate(&controller_drone_velocity_y, err_velocity_y, dt);

    

    // tranfer to bodyframe
    quaternion_2_euler(&drone_quaternion, &roll, &pitch, &yaw);
    cy = cosf(yaw);
    sy = sinf(yaw);

    ax_body = cy * accelerate_x + sy * accelerate_y;
    ay_body = -sy * accelerate_x + cy * accelerate_y;

    // tranfer to roll pitch ref
    pitch_ref = clampf(atanf(ax_body / DRONE_g), MIN_RAD_PITCH_REF, MAX_RAD_PITCH_REF);
    roll_ref  = clampf(atanf(-ay_body / DRONE_g), MIN_RAD_ROLL_REF, MAX_RAD_ROLL_REF);
}


void task_handle_attitude(task_data_t *data)
{
    float dt = data->dt;
    float roll, pitch, yaw;
    float err_roll, err_pitch, err_yaw;
    // handle pid for roll pitch yaw

    /*
        err_roll = roll_ref - sensor value
    */
    quaternion_2_euler(&drone_quaternion, &roll, &pitch, &yaw);
    err_pitch = pitch_ref - pitch;
    err_roll = roll_ref - roll;
    err_yaw = yaw_ref - yaw;
    // wrap yaw
    while (err_yaw > DRONE_Pi) err_yaw -= (2.0f*DRONE_Pi);
    while (err_yaw < -DRONE_Pi) err_yaw += (2.0f*DRONE_Pi);


    roll_rate_ref = pid_calculate(&controller_drone_roll, err_roll, dt);
    pitch_rate_ref = pid_calculate(&controller_drone_pitch, err_pitch, dt);
    yaw_rate_ref = pid_calculate(&controller_drone_yaw, err_yaw, dt);
}


void task_handle_rate(task_data_t *data)
{
    float dt = data->dt;
    float U1, U2, U3, U4;
    // handle for output
    float err_velocity_z = z_velocity_ref - drone_velocity.vz;
    float err_roll_rate = roll_rate_ref - drone_attitute.p;
    float err_pitch_rate = pitch_rate_ref - drone_attitute.q;
    float err_yaw_rate = yaw_rate_ref - drone_attitute.r;

    // U1 throttle z
    U1 = smc_calculate(&controller_drone_velocity_z, dt, err_velocity_z, z_velocity_ref, drone_velocity.vz, NULL, SMC_Z);

    // U2 Roll
    U2 = smc_calculate(&controller_drone_rate_roll, dt, err_roll_rate, roll_rate_ref, 0, &drone_attitute, SMC_ROLL);

    // U3 Pitch
    U3 = smc_calculate(&controller_drone_rate_pitch, dt, err_pitch_rate, pitch_rate_ref, 0, &drone_attitute, SMC_PITCH);

    // U4 yaw
    U4 = pid_calculate(&controller_drone_rate_yaw, err_yaw_rate, dt);


    // need to modify this function
    mixer_calculate(U1, U2, U3, U4);
}

void task_handle_barometer(task_data_t *data)
{
    float data_barometer;
    int check;
    
#ifdef SIMULATION_ON
    while (barometer_dps310_read_2_height(&data_barometer) != 0);
#else

    check = barometer_dps310_read_2_height(&data_barometer);
    send_cmd_read_dps310();
    if (check != 0)
        return;
#endif
    esekf_update_with_barometer(P, data_barometer);
}


void task_handle_compass(task_data_t *data)
{
    float compass_yaw;
    float roll, pitch, yaw;
    int check = 1;

    // quaternion_2_euler(&drone_quaternion, &roll, &pitch, &yaw);
    // // handle error when read error
    // check = be880_read_compass_2_yaw(roll, pitch, &compass_yaw);
    // be880_send_cmd_read_compass();

    if (check != 0)
        return;
    
#ifdef SIMULATION_ON
    test_yaw = compass_yaw;
#endif

    esekf_update_with_compass(P, compass_yaw, yaw);
}


/*

    some Background task

*/

