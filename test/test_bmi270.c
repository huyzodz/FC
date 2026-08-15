#include "test_bmi270.h"
#include "imu_bmi270.h"
#include "timer.h"
#include "i2c_master.h"


imu_data_t data_read;
float acc_x, acc_y, acc_z;
float gryo_x, gryo_y, gryo_z;


uint8_t acc_conf, acc_range, gyr_conf, gyr_range;


void test_bmi270(void)
{
    bmi270_calib();


    while (1)
    {
		delay_ms(100);
        //acc_range = i2c_read_reg(0x68, 0x41, I2C_NUM_1);
        bmi270_read(&data_read, 1);
		gryo_x = BMI270_GYRO_2_RAD(data_read.gyrox);
		
		acc_x = BMI270_ACC_2_MS2(data_read.accx);
		acc_y = BMI270_ACC_2_MS2(data_read.accy);
		acc_z = BMI270_ACC_2_MS2(data_read.accz);
        
    }
}