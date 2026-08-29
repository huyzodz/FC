#ifndef _MATRIX_H_
#define _MATRIX_H_


#include <stdint.h>



inline void matrix_product_3_block(const float *MatrixA, const float *MatrixB, float *ret);
inline void matix_add_3_block(const float *MatrixA, const float *MatrixB, float *ret);

inline void move_3_to_block(float *dst, const float *src, uint8_t stride);


#endif 