#include "esekf.h"
#include <math.h>


float ax_w, ay_w, az_w;

void convert_2_quaternion(quaternion_t *ret, quaternion_t qk, imu_data_t data, float dt)
{
    // gryo use only
    float alpha, alpha_q;
    quaternion_t delta_q;
    float ux, uy, uz;
    ux = (float)data.gyrox*dt;
    uy = (float)data.gyroy*dt;
    uz = (float)data.gyroz*dt;
    alpha = (float)sqrt(pow(ux,2) + pow(uy,2) + pow(uz,2));

    // if not rotate
    if (alpha < 1e-8f)
    {
        *ret = qk;
        return;
    }
    // standardization
    ux /= alpha;
    uy /= alpha;
    uz /= alpha;

    //calculate delta quaternion
    delta_q.w = cos(alpha/2);
    delta_q.x = ux*sin(alpha/2);
    delta_q.y = uy*sin(alpha/2);
    delta_q.z = uz*sin(alpha/2);

    // multiply 2 quaternion
    // qk * delta_q
    /*
    w = w1*w2 - x1*x2 - y1*y2 - z1*z2
    x = w1*x2 + x1*w2 + y1*z2 - z1*y2
    y = w1*y2 - x1*z2 + y1*w2 + z1*x2
    z = w1*z2 + x1*y2 - y1*x2 + z1*w2
    */
    ret->w = qk.w*delta_q.w - qk.x*delta_q.x - qk.y*delta_q.y - qk.z*delta_q.z;
    ret->x = qk.w*delta_q.x + qk.x*delta_q.w + qk.y*delta_q.z - qk.z*delta_q.y;
    ret->y = qk.w*delta_q.y - qk.x*delta_q.z + qk.y*delta_q.w + qk.z*delta_q.x;
    ret->z = qk.w*delta_q.z + qk.x*delta_q.y - qk.y*delta_q.x + qk.z*delta_q.w;

    alpha_q = sqrt(pow(ret->w, 2) + pow(ret->x, 2) + pow(ret->y, 2) + pow(ret->z, 2));

    // standardization
    if (alpha_q > 1e-8f)
    {
        ret->w /= alpha_q;
        ret->x /= alpha_q;
        ret->y /= alpha_q;
        ret->z /= alpha_q;
    }
    
}


void convert_2_velocity(velocity_t *ret, velocity_t vk, quaternion_t q, imu_data_t data, float dt)
{
    /*
    
        v = v + (R(amv − ab) + g) ∆t;
        R is rotation matrix of quaternion
    */
    /*
        amv-ab has been calculated in imu read function
        below is (R(amv − ab))
        this is rotation
    */
    ax_w =
    (1.0f - 2.0f * (q.y*q.y + q.z*q.z)) * data.accx
    + 2.0f * (q.x*q.y - q.z*q.w) * data.accy
    + 2.0f * (q.x*q.z + q.y*q.w) * data.accz;

    ay_w =
    2.0f * (q.x*q.y + q.z*q.w) * data.accx
    + (1.0f - 2.0f * (q.x*q.x + q.z*q.z)) * data.accy
    + 2.0f * (q.y*q.z - q.x*q.w) * data.accz;

    az_w =
    2.0f * (q.x*q.z - q.y*q.w) * data.accx
    + 2.0f * (q.y*q.z + q.x*q.w) * data.accy
    + (1.0f - 2.0f * (q.x*q.x + q.y*q.y)) * data.accz;

    // below is intergral from acc to velocity DRONE_g is minus or add base on z line
    ret->vx = vk.vx + ax_w*dt;
    ret->vy = vk.vy + ay_w*dt;
    ret->vz = vk.vz + (az_w - DRONE_g)*dt;
}

void convert_2_position(position_t *ret, position_t pk, velocity_t vk, float dt)
{
    ret->x = pk.x + vk.vx*dt + (float)0.5f*ax_w*dt*dt;
    ret->y = pk.y + vk.vy*dt + (float)0.5f*ay_w*dt*dt;
    ret->z = pk.z + vk.vz*dt + (float)0.5f*(az_w - DRONE_g)*dt*dt;
}