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
static inline void matrix_product_3_block_add(const float *MatrixA, const float *MatrixB, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[i * 3 + 0] * MatrixB[0 * 3 + j] + MatrixA[i * 3 + 1] * MatrixB[1 * 3 + j] + MatrixA[i * 3 + 2] * MatrixB[2 * 3 + j];
       
}

// operate only -(A*B)
static inline void matrix_product_3_block_minus(const float *MatrixA, const float *MatrixB, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = -(MatrixA[i * 3 + 0] * MatrixB[0 * 3 + j] + MatrixA[i * 3 + 1] * MatrixB[1 * 3 + j] + MatrixA[i * 3 + 2] * MatrixB[2 * 3 + j]);
       
}

static inline void matrix_add_3_block(const float *MatrixA, const float *MatrixB, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[i * 3 + j] + MatrixB[i * 3 + j];
}

static inline void move_3_to_block(float *dst, const float *src, uint8_t stride)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            dst[i * stride + j] = src[i * 3 +j];
}


static inline void num_product_matrix_3block(const float num, const float *matrix, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = num*matrix[i * 3 + j];

}


static inline void matrix_3_block_tranpose(const float *A, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[j * 3 + i] = A[i * 3 + j];   
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
    };
}


// this function use word frame xyz
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




void esekf_imu_covariance(quaternion_t qua, float dt, imu_data_t imu_data, matrix_esekf_t ret[5][5])
{
    matrix_esekf_t temp[5][5] = {0};
    float R[3][3];
    quaternion_to_rotation(qua, R);

    float A[3][3];
    esekf_3_skew(imu_data, A, dt);
    float I3[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    float A3[3][3] = {{dt,0,0},{0,dt,0},{0,0,dt}};

    float B3[3][3];
    matrix_product_3_block_minus(R, A, B3); // = -R*A
    float B3_T[3][3];
    matrix_3_block_tranpose(B3, B3_T);

    float C3[3][3]; // = -R*dt
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            C3[i][j] = -R[i][j]*dt;
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



                    // temp = Fx*P
    // row 1
    for (int i = 0;i < 5;i++)
    {
        
        float R0Ci[3][3];
        float R1Ci[3][3];
        matrix_product_3_block_add(I3, ret[0][i].data, R0Ci);
        matrix_product_3_block_add(A3, ret[1][i].data, R1Ci);

        matrix_add_3_block(R0Ci, R1Ci, temp[0][i].data);
    }

    // row 2
    for (int i = 0;i < 5;i++)
    {
        float R1Ci[3][3];
        float R2Ci[3][3];
        float R3Ci[3][3];

        matrix_product_3_block_add(I3, ret[1][i].data, R1Ci);
        matrix_product_3_block_add(B3, ret[2][i].data, R2Ci);
        matrix_product_3_block_add(C3, ret[3][i].data, R3Ci);

        // sum result
        for (int x = 0;x < 3;x++)
            for (int y = 0;y < 3;y++)
                temp[1][i].data[x][y] = R1Ci[x][y] + R2Ci[x][y] + R3Ci[x][y];
    }
    

    // row 3
    for (int i = 0; i < 5; i++)
    {
        float R2Ci[3][3];
        float R4Ci[3][3];

        matrix_product_3_block_add(D3, ret[2][i].data, R2Ci);
        matrix_product_3_block_add(A3, ret[4][i].data, R4Ci);

        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                temp[2][i].data[x][y] = R2Ci[x][y] - R4Ci[x][y];
    }

    // row 4
    for (int i = 0; i < 5; i++)
        memcpy(temp[3][i].data, ret[3][i].data, sizeof(temp[3][i].data));

    // row 5
    for (int i = 0; i < 5; i++)
        memcpy(temp[4][i].data, ret[4][i].data, sizeof(temp[4][i].data));



    //                  P*FxT
    // col 1
    for (int i = 0;i < 5;i++)
    {
        float RiC0[3][3];
        float RiC1[3][3];
        matrix_product_3_block_add(temp[i][0].data, I3, RiC0);
        matrix_product_3_block_add(temp[i][1].data, A3, RiC1);

        matrix_add_3_block(RiC0, RiC1, ret[i][0].data);
    }

    // col 2
    for (int i = 0;i < 5;i++)
    {
        float RiC1[3][3];
        float RiC2[3][3];
        float RiC3[3][3];

        matrix_product_3_block_add(temp[i][1].data, I3, RiC1);
        matrix_product_3_block_add(temp[i][2].data, B3_T, RiC2);
        matrix_product_3_block_add(temp[i][3].data, C3_T, RiC3);

        // sum result
        for (int x = 0;x < 3;x++)
            for (int y = 0;y < 3;y++)
                ret[i][1].data[x][y] = RiC1[x][y] + RiC2[x][y] + RiC3[x][y];
    }

    // col 3
    for (int i = 0;i < 5;i++)
    {
        float RiC2[3][3];
        float RiC4[3][3];

        matrix_product_3_block_add(temp[i][2].data, D3_T, RiC2);
        matrix_product_3_block_add(temp[i][4].data, A3, RiC4);

        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                ret[i][2].data[x][y] = RiC2[x][y] - RiC4[x][y];
    }

    // col 4
    for (int i = 0; i < 5; i++)
        memcpy(ret[i][3].data, temp[i][3].data, sizeof(temp[i][3].data));
        

    // col 5
    for (int i = 0; i < 5; i++)
        memcpy(ret[i][4].data, temp[i][4].data, sizeof(temp[i][4].data));

    // add with Qw
    for (int i = 1;i < 5;i++)
        matrix_add_3_block(ret[i][i].data, Qw[i-1].data, ret[i][i].data);
}
