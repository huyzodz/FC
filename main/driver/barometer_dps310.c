#include "barometer_dps310.h"
#include "math.h"
#include "timer.h"

#include "define_sim.h"
#include "simulate.h"

volatile float baromater_base;


static int barometer_dps310_read_raw(float *ret)
{
#ifdef SIMULATION_ON
    simulate_data_rx_t data_temp;
    if(simulate_getData(&data_temp) != 0) return 1;
    *ret = data_temp.press;
#else

#endif

    return 0;
}


void barometer_dps310_init(void)
{
#ifndef SIMULATION_ON

    // init with i2c
    // init with real device here

#endif
    float sum = 0;
    int i = 0;
    while (i < 10)
    {
        float temp = 0;
        if (barometer_dps310_read_raw(&temp) != 0) continue;
        sum += temp;
        i++;
        delay_ms(100);

    }
    baromater_base = (float)(sum / 10);
}


int barometer_dps310_read_2_height(float *ret)
{
    float temp;
    if (barometer_dps310_read_raw(&temp) != 0) return 1;
    // tranfer to height

    *ret = 44330*(powf((baromater_base/temp), 0.1903f) - 1);
    return 0;
}