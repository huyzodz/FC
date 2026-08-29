#include <stdint.h>
#include <stdio.h>


float P[3][3];


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

int main(void)
{
    float A[3][3] =
    {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f}
    };

    float B[3][3] =
    {
        {9.0f, 8.0f, 7.0f},
        {6.0f, 5.0f, 4.0f},
        {3.0f, 2.0f, 1.0f}
    };
    matrix_add_3_block(A, B, P);

    return 0;
}