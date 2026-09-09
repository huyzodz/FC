#ifndef _GPS_BE880_H_
#define _GPS_BE880_H_

typedef struct {
    float x;
    float y;
    float z;
} compass_data_t;

typedef enum {
    BE880_NO_COMPASS = 0,
    BE880_HAS_COMPASS
} be880_type_t;


void gps_be880_init(be880_type_t cfg);

int be880_read_compass(compass_data_t *ret);
int be880_read_compass_2_yaw(float roll, float pitch, float *ret);

#endif