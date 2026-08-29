#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "stdio.h"

typedef enum {
    CLOCK_8_MHZ = 0,
    CLOCK_16_MHZ,
    CLOCK_32_MHZ,
    CLOCK_64_MHZ, // switch from hsi to pll
    CLOCK_80_MHZ,
    CLOCK_120_MHZ,
    CLOCK_160_MHZ,
    CLOCK_200_MHZ,
    CLOCK_240_MHZ,
    CLOCK_280_MHZ,
    CLOCK_320_MHZ,
    CLOCK_360_MHZ,
    CLOCK_400_MHZ,
    CLOCK_440_MHZ,
    CLOCK_480_MHZ,
} cpu_clock_t;


/* must call at first place in project */
int clock_init(cpu_clock_t clock);



#endif
