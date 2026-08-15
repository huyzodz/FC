#ifndef _GPS_BE880_H_
#define _GPS_BE880_H_



typedef enum {
    BE880_NO_COMPASS = 0,
    BE880_HAS_COMPASS
} be880_type_t;


void gps_be880_init(be880_type_t cfg);



#endif