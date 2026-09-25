#ifndef _SIMULATE_H_
#define _SIMULATE_H_

#include "imu_bmi270.h"
#include "mixer.h"


#define SIMULATE_COM_UART_NUM               USART_2


extern uint8_t temp_sim [10];
extern motor_output_t send;

// in this file use uart to communicate data between gazebo and stm32

#pragma pack(push, 1)
typedef struct {
    uint8_t header;
    float accx;
    float accy;
    float accz;
    float gyrox;
    float gyroy;
    float gyroz;
    float press;
} simulate_data_rx_t;
#pragma pack(pop)



extern float test_yaw;
extern uint8_t temp_compass_i2c [6];

void simulation_init(void);

int simulate_getData(simulate_data_rx_t *ret);


void test_simulation(void);

#endif