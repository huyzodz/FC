#include "esekf.h"
#include <math.h>
#include <string.h>


/*                              CHECK AGAIN D3 in covariance IF STH WRONG                 */


#define SIGMA_V_SQUARE                  1.0f
#define SIGMA_PHI_SQUARE                1.0f
#define SIGMA_A_SQUARE                  1.0f
#define SIGMA_W_SQUARE                  1.0f


float ax_w, ay_w, az_w;
// use in esekf

// matrix_esekf_t Qw[5][5];

/*
    some function use only in this file
*/



// operate only +A*+B
static inline void matrix_product_3_block_add(const float MatrixA[3][3], const float MatrixB[3][3], float ret[3][3])
{
    int i = 0;
    while (i < 3)
    {
        int j = 0;
        while (j < 3)
        {
            ret[i][j] = MatrixA[i][0] * MatrixB[0][j] + MatrixA[i][1] * MatrixB[1][j] + MatrixA[i][2] * MatrixB[2][j];
            j++;
        }
        i++;
    }
}

// operate only -(A*B)
static inline void matrix_product_3_block_minus(const float MatrixA[3][3], const float MatrixB[3][3], float ret[3][3])
{
    int i = 0;
    while (i < 3)
    {
        int j = 0;
        while (j < 3)
        {
            ret[i][j] = -(MatrixA[i][0] * MatrixB[0][j] + MatrixA[i][1] * MatrixB[1][j] + MatrixA[i][2] * MatrixB[2][j]);
            j++;
        }
        i++;
    }       
}

static inline void matrix_add_3_block(const float A[3][3], const float B[3][3], float ret[3][3])
{
    ret[0][0] = A[0][0] + B[0][0];
    ret[0][1] = A[0][1] + B[0][1];
    ret[0][2] = A[0][2] + B[0][2];

    ret[1][0] = A[1][0] + B[1][0];
    ret[1][1] = A[1][1] + B[1][1];
    ret[1][2] = A[1][2] + B[1][2];

    ret[2][0] = A[2][0] + B[2][0];
    ret[2][1] = A[2][1] + B[2][1];
    ret[2][2] = A[2][2] + B[2][2];
}

static inline void move_3_to_block(float dst[3][3], const float src[3][3], uint8_t stride)
{
    int i = 0;
    while (i < 3)
    {
        int j = 0;
        while (j < 3)
        {
            dst[i][j] = src[i][j];
            j++;
        }
        i++;
    }           
}


static inline void num_product_matrix_3block(const float num, const float matrix[3][3], float ret[3][3])
{
    int i = 0;
    while (i < 3)
    {
        int j = 0;
        while (j < 3)
        {
            ret[i][j] = num * matrix[i][j];
            j++;
        }
        i++;
    }        
}


static inline void matrix_3_block_tranpose(const float A[3][3], float ret[3][3])
{
    int i = 0;
    while (i < 3)
    {
        int j = 0;
        while (j < 3)
        {
            ret[j][i] = A[i][j];  
            j++;
        }
        i++;
    }   
}


static inline void esekf_3_skew(imu_data_t data, float ret[3][3], float dt)
{
    const float x = data.accx*dt;
    const float y = data.accy*dt;
    const float z = data.accz*dt;

    ret[0][0] =  0.0f;
    ret[0][1] = -z;
    ret[0][2] =  y;

    ret[1][0] =  z;
    ret[1][1] =  0.0f;
    ret[1][2] = -x;

    ret[2][0] = -y;
    ret[2][1] =  x;
    ret[2][2] =  0.0f;
}

static inline void quaternion_to_rotation(quaternion_t qua, float R[3][3])
{
    float w = qua.w;
    float x = qua.x;
    float y = qua.y;
    float z = qua.z;

    float xx = x * x;
    float yy = y * y;
    float zz = z * z;

    float xy = x * y;
    float xz = x * z;
    float yz = y * z;

    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    R[0][0] = 1.0f - 2.0f * (yy + zz);
    R[0][1] = 2.0f * (xy - wz);
    R[0][2] = 2.0f * (xz + wy);

    R[1][0] = 2.0f * (xy + wz);
    R[1][1] = 1.0f - 2.0f * (xx + zz);
    R[1][2] = 2.0f * (yz - wx);

    R[2][0] = 2.0f * (xz - wy);
    R[2][1] = 2.0f * (yz + wx);
    R[2][2] = 1.0f - 2.0f * (xx + yy);
}

/*

    ******************************

*/


void convert_2_quaternion(quaternion_t *ret, const quaternion_t *qk, const imu_data_t *data, float dt)
{
    // gryo use only
    float alpha, alpha_q;
    quaternion_t delta_q;
    float ux, uy, uz;
    ux = (float)data->gyrox*dt;
    uy = (float)data->gyroy*dt;
    uz = (float)data->gyroz*dt;
    alpha = (float)sqrt(pow(ux,2) + pow(uy,2) + pow(uz,2));

    // if not rotate
    if (alpha < 1e-8f)
    {
        *ret = *qk;
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
    ret->w = qk->w*delta_q.w - qk->x*delta_q.x - qk->y*delta_q.y - qk->z*delta_q.z;
    ret->x = qk->w*delta_q.x + qk->x*delta_q.w + qk->y*delta_q.z - qk->z*delta_q.y;
    ret->y = qk->w*delta_q.y - qk->x*delta_q.z + qk->y*delta_q.w + qk->z*delta_q.x;
    ret->z = qk->w*delta_q.z + qk->x*delta_q.y - qk->y*delta_q.x + qk->z*delta_q.w;

    alpha_q = sqrtf(pow(ret->w, 2) + pow(ret->x, 2) + pow(ret->y, 2) + pow(ret->z, 2));

    // standardization
    if (alpha_q > 1e-8f)
    {
        ret->w /= alpha_q;
        ret->x /= alpha_q;
        ret->y /= alpha_q;
        ret->z /= alpha_q;
    };
}


// this function use word frame xyz
void convert_2_velocity(velocity_t *ret, const velocity_t *vk, const quaternion_t *q, const imu_data_t *data, float dt)
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
    float w = q->w, x = q->x, y = q->y, z = q->z;
    float ax = data->accx, ay = data->accy, az = data->accz;

    float xx2 = 2.0f * x * x;
    float yy2 = 2.0f * y * y;
    float zz2 = 2.0f * z * z;

    float xy2 = 2.0f * x * y;
    float xz2 = 2.0f * x * z;
    float yz2 = 2.0f * y * z;

    float wx2 = 2.0f * w * x;
    float wy2 = 2.0f * w * y;
    float wz2 = 2.0f * w * z;


    ax_w = (1.0f - yy2 - zz2) * ax + (xy2 - wz2) * ay + (xz2 + wy2) * az;
    ay_w = (xy2 + wz2) * ax + (1.0f - xx2 - zz2) * ay + (yz2 - wx2) * az;
    az_w  = (xz2 - wy2) * ax + (yz2 + wx2) * ay + (1.0f - xx2 - yy2) * az;

    // ax_w =
    // (1.0f - 2.0f * (q->y*q->y + q->z*q->z)) * data->accx
    // + 2.0f * (q->x*q->y - q->z*q->w) * data->accy
    // + 2.0f * (q->x*q->z + q->y*q->w) * data->accz;

    // ay_w =
    // 2.0f * (q.x*q.y + q.z*q.w) * data.accx
    // + (1.0f - 2.0f * (q.x*q.x + q.z*q.z)) * data.accy
    // + 2.0f * (q.y*q.z - q.x*q.w) * data.accz;

    // az_w =
    // 2.0f * (q.x*q.z - q.y*q.w) * data.accx
    // + 2.0f * (q.y*q.z + q.x*q.w) * data.accy
    // + (1.0f - 2.0f * (q.x*q.x + q.y*q.y)) * data.accz;

    // below is intergral from acc to velocity DRONE_g is minus or add base on z line
    ret->vx = vk->vx + ax_w*dt;
    ret->vy = vk->vy + ay_w*dt;
    ret->vz = vk->vz + (az_w - DRONE_g)*dt;
}

void convert_2_position(position_t *ret, const position_t *pk, const velocity_t *vk, float dt)
{
    ret->x = pk->x + vk->vx*dt + (float)0.5f*ax_w*dt*dt;
    ret->y = pk->y + vk->vy*dt + (float)0.5f*ay_w*dt*dt;
    ret->z = pk->z + vk->vz*dt + (float)0.5f*(az_w - DRONE_g)*dt*dt;
}




void esekf_imu_covariance(quaternion_t qua, float dt, imu_data_t imu_data, matrix_esekf_t ret[5][5])
{
    int i = 0, j = 0;
    matrix_esekf_t temp[5][5] = {0};
    size_t size = sizeof(temp[0][3].data);
    float R[3][3];
    quaternion_to_rotation(qua, R);

    float A[3][3];
    esekf_3_skew(imu_data, A, dt);
    //float I3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    //float A3[3][3] = {{dt,0,0},{0,dt,0},{0,0,dt}};

    float B3[3][3];
    matrix_product_3_block_minus(R, A, B3); // = -R*A
    float B3_T[3][3];
    matrix_3_block_tranpose(B3, B3_T);

    float C3[3][3]; // = -R*dt
    i = 0;j = 0;
    while (i < 3)
    {
        j = 0;
        while (j < 3)
        {
            C3[i][j] = -R[i][j]*dt;
            j++;
        }
        i++;
    }  
    float C3_T[3][3];
    matrix_3_block_tranpose(C3, C3_T);

    float omega_x = imu_data.gyrox*dt;
    float omega_y = imu_data.gyroy*dt;
    float omega_z = imu_data.gyroz*dt;
    // linear first-order approximation matrix
    float D3[3][3] = {{1,+omega_z,-omega_y},{-omega_z, 1, +omega_x},{+omega_y,-omega_x,1}} ; // = R'{w}
    float D3_T[3][3];
    matrix_3_block_tranpose(D3, D3_T);
    

    // update Qw
    // Qw here is Fw*Qw*Fw_T
    float dt2 = dt * dt;

    float qv = SIGMA_V_SQUARE * dt2;
    float qt = SIGMA_PHI_SQUARE * dt2;
    float qa = SIGMA_A_SQUARE * dt2;
    float qw = SIGMA_W_SQUARE * dt2;
    /*
    matrix_esekf_t Qw[4] = {0};
    // sigma_v^2 * dt^2 * I3
    Qw[0].data[0][0] = qv;
    Qw[0].data[1][1] = qv;
    Qw[0].data[2][2] = qv;

    // sigma_theta^2 * dt^2 * I3
    Qw[1].data[0][0] = qt;
    Qw[1].data[1][1] = qt;
    Qw[1].data[2][2] = qt;

    // sigma_a^2 * dt^2 * I3
    Qw[2].data[0][0] = qa;
    Qw[2].data[1][1] = qa;
    Qw[2].data[2][2] = qa;

    // sigma_w^2 * dt^2 * I3
    Qw[3].data[0][0] = qw;
    Qw[3].data[1][1] = qw;
    Qw[3].data[2][2] = qw;
    */


                    // temp = Fx*P
    // row 1
    i = 0;
    while (i < 5)
    {
        // float R0Ci[3][3];
        // float R1Ci[3][3];
        // matrix_product_3_block_add(I3, ret[0][i].data, R0Ci);
        // matrix_product_3_block_add(A3, ret[1][i].data, R1Ci);

        // matrix_add_3_block(R0Ci, R1Ci, temp[0][i].data);
        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                temp[0][i].data[x][y] = ret[0][i].data[x][y] + dt * ret[1][i].data[x][y];
                y++;
            }
            x++;
        }
        i++;
    }

    // row 2
    i = 0;
    while (i < 5)
    {
        //float R1Ci[3][3];
        float R2Ci[3][3];
        float R3Ci[3][3];

        //matrix_product_3_block_add(I3, ret[1][i].data, R1Ci);
        matrix_product_3_block_add(B3, ret[2][i].data, R2Ci);
        matrix_product_3_block_add(C3, ret[3][i].data, R3Ci);

        // sum result
        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                temp[1][i].data[x][y] = ret[1][i].data[x][y] + R2Ci[x][y] + R3Ci[x][y];
                y++;
            }
            x++;
        }              
        i++;
    }
    

    // row 3
    i = 0;
    while (i < 5)
    {
        float R2Ci[3][3];
        //float R4Ci[3][3];

        matrix_product_3_block_add(D3, ret[2][i].data, R2Ci);
        //matrix_product_3_block_add(A3, ret[4][i].data, R4Ci);

        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                //temp[2][i].data[x][y] = R2Ci[x][y] - R4Ci[x][y];
                temp[2][i].data[x][y] = R2Ci[x][y] - dt*ret[4][i].data[x][y];
                y++;
            }
            x++;
        }
        i++;
    }

    // row 4
    i = 0;
    while (i < 5)
    {
        memcpy(temp[3][i].data, ret[3][i].data, size);
        //move_3_to_block(temp[3][i].data, ret[3][i].data, 0);
        i++;
    }
        

    // row 5
    i = 0;
    while (i < 5)
    {
        memcpy(temp[4][i].data, ret[4][i].data, size);
        //move_3_to_block(temp[4][i].data, ret[4][i].data, 0);
        i++;
    }



    //                  P*FxT
    // col 1
    i = 0;
    while (i < 5)
    {
        // float RiC0[3][3];
        // float RiC1[3][3];
        // matrix_product_3_block_add(temp[i][0].data, I3, RiC0);
        // matrix_product_3_block_add(temp[i][1].data, A3, RiC1);

        // matrix_add_3_block(RiC0, RiC1, ret[i][0].data);
        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                ret[i][0].data[x][y] = temp[i][0].data[x][y] + dt * temp[i][1].data[x][y];
                y++;
            }
            x++;
        }
        i++;
    }

    // col 2
    i = 0;
    while (i < 5)
    {
        //float RiC1[3][3];
        float RiC2[3][3];
        float RiC3[3][3];

        //matrix_product_3_block_add(temp[i][1].data, I3, RiC1);
        matrix_product_3_block_add(temp[i][2].data, B3_T, RiC2);
        matrix_product_3_block_add(temp[i][3].data, C3_T, RiC3);

        // sum result
        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                //ret[i][1].data[x][y] = RiC1[x][y] + RiC2[x][y] + RiC3[x][y];
                ret[i][1].data[x][y] = temp[i][1].data[x][y] + RiC2[x][y] + RiC3[x][y];
                y++;
            }
            x++;
        } 
        i++;
    }

    // col 3
    i = 0;
    while (i < 5)
    {
        float RiC2[3][3];
        //float RiC4[3][3];

        matrix_product_3_block_add(temp[i][2].data, D3_T, RiC2);
        //matrix_product_3_block_add(temp[i][4].data, A3, RiC4);

        int x = 0;
        while (x < 3)
        {
            int y = 0;
            while (y < 3)
            {
                //ret[i][2].data[x][y] = RiC2[x][y] - RiC4[x][y];
                ret[i][2].data[x][y] = RiC2[x][y] - temp[i][4].data[x][y]*dt;
                y++;
            }
            x++;
        } 
        i++;
    }

    // col 4
    i = 0;
    while (i < 5)
    {
        memcpy(ret[i][3].data, temp[i][3].data, size);
        //move_3_to_block(ret[i][3].data, temp[i][3].data, 0);
        i++;
    }
        
        

    // col 5
    i = 0;
    while (i < 5)
    {
        //move_3_to_block(ret[i][4].data, temp[i][4].data, 0);
        memcpy(ret[i][4].data, temp[i][4].data, size);
        i++;
    }
    // add with Qw
    /*
    i = 1;
    while (i < 5)
    {
        int temp1 = i - 1;
        //matrix_add_3_block(ret[i][i].data, Qw[temp1].data, ret[i][i].data);
        // int x = 0;
        // while (x < 3)
        // {
        //     ret[i][i].data[x][x] = ret[i][i].data[x][x] + Qw[temp1].data[x][x];
        //     x++;
        // }
        
        i++;
    }
    */
    ret[1][1].data[0][0] += qv;
    ret[1][1].data[1][1] += qv;
    ret[1][1].data[2][2] += qv;

    ret[2][2].data[0][0] += qt;
    ret[2][2].data[1][1] += qt;
    ret[2][2].data[2][2] += qt;

    ret[3][3].data[0][0] += qa;
    ret[3][3].data[1][1] += qa;
    ret[3][3].data[2][2] += qa;

    ret[4][4].data[0][0] += qw;
    ret[4][4].data[1][1] += qw;
    ret[4][4].data[2][2] += qw;

}


void quaternion_2_euler(const quaternion_t *q, float *roll, float *pitch, float *yaw)
{
    // limit to not NaN in pitch
    float val = 2.0f * (q->w * q->y - q->z * q->x);
    if (val > 1.0f)  val = 1.0f;
    if (val < -1.0f) val = -1.0f;

    *roll = atan2f(2*(q->w*q->x + q->y*q->z), 1 - 2*(q->x*q->x + q->y*q->y));
    *pitch = asinf(val);
    *yaw  = atan2f(2*(q->w*q->z + q->y*q->x), 1 - 2*(q->z*q->z + q->y*q->y));
}