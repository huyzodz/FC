#include "matrix.h"
#include <malloc.h>


inline void matrix_product_3_block(const float *MatrixA, const float *MatrixB, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[i * 3 + 0] * MatrixB[0 * 3 + j] + MatrixA[i * 3 + 1] * MatrixB[1 * 3 + j] + MatrixA[i * 3 + 2] * MatrixB[2 * 3 + j];
}


inline void matix_add_3_block(const float *MatrixA, const float *MatrixB, float *ret)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[i * 3 + j] + MatrixB[i * 3 + j];
}

inline void move_3_to_block(float *dst, const float *src, uint8_t stride)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            dst[i * stride + j] = src[i * 3 +j];
}

// pointer here is only 15

inline void matrix_product_3block_pointer_B(const float *MatrixA, const float *MatrixB, float *ret, uint8_t loc)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[i * 3 + 0] * MatrixB[loc + j] + MatrixA[i * 3 + 1] * MatrixB[loc + 15 + j] + MatrixA[i * 3 + 2] * MatrixB[loc + 30 + j];
}

inline void matrix_product_3block_pointer_A(const float *MatrixA, const float *MatrixB, float *ret, uint8_t loc)
{
    for (int i = 0;i < 3;i++)
        for (int j = 0;j < 3;j++)
            ret[i * 3 + j] = MatrixA[(loc + i*15) + 0] * MatrixB[0 * 3 + j] + MatrixA[(loc + i*15) + 1] * MatrixB[1 * 3 + j] + MatrixA[(loc + i*15) + 2] * MatrixB[2 * 3 + j];
}