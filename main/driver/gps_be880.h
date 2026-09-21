#ifndef _GPS_BE880_H_
#define _GPS_BE880_H_

#include <stdint.h>

#define COMPASS_SIZE_READ                           6

typedef struct {
    float x;
    float y;
    float z;
} compass_data_t;

typedef enum {
    BE880_NO_COMPASS = 0,
    BE880_HAS_COMPASS
} be880_type_t;


volatile extern uint8_t compass_data_ram [COMPASS_SIZE_READ * 2];


void gps_be880_init(be880_type_t cfg);

int be880_send_cmd_read_compass(void);
int be880_read_compass_2_yaw(float roll, float pitch, float *ret);
void be880_init_yaw_compas(float *yaw_init_ref);

#endif