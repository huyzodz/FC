#include "mixer.h"
#include "dshot.h"
#include "math.h"


#define NUM_MOTOR                           4

#define CONSTANT_Kf                         1.017e-5f
#define CONSTANT_Km                         0.02071f
#define CONSTANT_Km_DIV_Kf_ROTOR            0.02043f
#define CONSTANT_RAD_TO_RPM                 9.549297f



#define MAX_MOTOR_SPEED                     15275
#define MIN_MOTOR_SPEED                     5000                                                  



#define RPM_TO_DSHOT(val)                   (48 + (((val) - MIN_MOTOR_SPEED) * (2047-48))/(MAX_MOTOR_SPEED - MIN_MOTOR_SPEED) )


// check again Kf and Km and goto matlab to calculate again
// U must div Kf too
static const motorMixer_t mixerQuadX[] = {
    { 0.25f, -0.25f,  0.25f, -12.2369f },          // FRONT_R
    { 0.25f,  0.25f,  0.25f,  12.2369f },          // REAR_R
    { 0.25f,  0.25f, -0.25f, -12.2369f },          // FRONT_L
    { 0.25f, -0.25f, -0.25f,  12.2369f },          // REAR_L
};

volatile static motor_output_t glb_arr_motor;

void write_motor()
{
    // function
    dshot_write(
        glb_arr_motor.motor_front_r,
        glb_arr_motor.motor_rear_r,
        glb_arr_motor.motor_front_l,
        glb_arr_motor.motor_rear_l
    );
}


uint16_t limit_speed_motor(uint16_t val)
{
    if (val > MAX_MOTOR_SPEED)
        return MAX_MOTOR_SPEED;
    if (val < MIN_MOTOR_SPEED)
        return MIN_MOTOR_SPEED;
    return val;
}


// remember to handle negative
// remember to add time measure
void mixer_calculate(float U1, float U2, float U3, float U4)
{   
    // div for Kf to match with matrix using
    U1 /= CONSTANT_Kf; U2 /= CONSTANT_Kf; U3 /= CONSTANT_Kf; U4 /= CONSTANT_Kf;

    float force_front_r = mixerQuadX[0].throttle * U1 +
                          mixerQuadX[0].roll * U2 +
                          mixerQuadX[0].pitch * U3 +
                          mixerQuadX[0].yaw * U4; 

    // same with above but increase position arr
    float force_rear_r = mixerQuadX[1].throttle * U1 +
                         mixerQuadX[1].roll * U2 +
                         mixerQuadX[1].pitch * U3 +
                         mixerQuadX[1].yaw * U4;

    float force_front_l = mixerQuadX[2].throttle * U1 +
                          mixerQuadX[2].roll * U2 +
                          mixerQuadX[2].pitch * U3 +
                          mixerQuadX[2].yaw * U4;

    float force_rear_l = mixerQuadX[3].throttle * U1 +
                         mixerQuadX[3].roll * U2 +
                         mixerQuadX[3].pitch * U3 +
                         mixerQuadX[3].yaw * U4;

    // force -> rad/s -> RPM
    float speed_front_r = sqrt(fabsf(force_front_r)) * CONSTANT_RAD_TO_RPM;
    float speed_rear_r = sqrt(fabsf(force_rear_r)) * CONSTANT_RAD_TO_RPM;
    float speed_front_l = sqrt(fabsf(force_front_l)) * CONSTANT_RAD_TO_RPM;
    float speed_rear_l = sqrt(fabsf(force_rear_l)) * CONSTANT_RAD_TO_RPM;
    

    // limit speed
    speed_front_r = limit_speed_motor(speed_front_r);
    speed_rear_r = limit_speed_motor(speed_rear_r);
    speed_front_l = limit_speed_motor(speed_front_l);
    speed_rear_l = limit_speed_motor(speed_rear_l);

    // convert to dshot
    glb_arr_motor.motor_front_r = RPM_TO_DSHOT(speed_front_r);
    glb_arr_motor.motor_rear_r = RPM_TO_DSHOT(speed_rear_r);
    glb_arr_motor.motor_front_l = RPM_TO_DSHOT(speed_front_l);
    glb_arr_motor.motor_rear_l = RPM_TO_DSHOT(speed_rear_l);


    // write to motor
    write_motor();
}

