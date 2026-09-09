#ifndef _BAROMETER_DPS310_H_
#define _BAROMETER_DPS310_H_


void barometer_dps310_init(void);

int barometer_dps310_read_2_height(float *ret);



#endif