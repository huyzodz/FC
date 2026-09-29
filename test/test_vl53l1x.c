#include "test.h"
#include <stdint.h>
#include "timer.h"
#include "vl53l1x.h"

volatile int16_t data_vl;
uint8_t RangeStatus;
uint32_t time_test;
uint16_t raw_vl;

void test_vl53l1x(void)
{
	START_SYSTEM_TIMER();
    uint8_t dev = 0x29;
	VL53L1X_ClearInterrupt(dev);
	delay_ms(10);
	
    // while (1)
    // {
	// 	uint8_t dataReady = 0;
	// 	// while (dataReady == 0)
	// 	// {
	// 	// 	VL53L1X_CheckForDataReady(dev, &dataReady);
	// 	// 	delay_ms(5);
	// 	// }
	// 	// delay_ms(5);
    //     // VL53L1X_GetRangeStatus(dev, &RangeStatus);
    //     if (VL53L1X_GetDistance(dev, &data_vl) != 0)
    //         continue;
    //     VL53L1X_ClearInterrupt(dev);
	// 	delay_ms(10);
    // }

	while (1)
	{
		uint32_t start = GET_CURRENT_US();
		uint8_t dataReady = 0;
		while (dataReady == 0)
		{
			VL53L1X_CheckForDataReady(dev, &dataReady);
			delay_ms(5);
		}
		VL53L1X_GetRangeStatus(dev, &RangeStatus);
		vl53l1x_send_cmd();
		while (vl53l1x_read_data(&data_vl) == -1);
		time_test = GET_CURRENT_US() - start;
	}
}