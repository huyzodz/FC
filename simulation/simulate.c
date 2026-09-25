#include "simulate.h"
#include "usart.h"
#include <string.h>
#include "timer.h"



#define SIMULATE_COM_PORT                   GPIO_PORT_D
#define SIMULATE_COM_RX                     6
#define SIMULUATE_COM_TX                    5

#define SIMULATE_RAM_SIZE                   29

volatile uint8_t simulate_data_ram [SIMULATE_RAM_SIZE];
volatile simulate_data_rx_t data;
float test_yaw;
uint8_t temp_compass_i2c [6];

uint8_t temp_sim [10];
motor_output_t send;

void simulation_init(void)
{
    usart_config_t usart_cfg = {
        .baudrate = USART_BAUDRATE_921600,
        .intr_en = USART_FALSE,
        .msb = USART_FALSE,
        .parity = USART_NONE_PARITY,
        .stop_bit = USART_1_STOP_BITS,
        .usart_num = SIMULATE_COM_UART_NUM,
        .word_length = USART_8_BITS,
        .gpio_port = SIMULATE_COM_PORT,
        .pin_rx = SIMULATE_COM_RX,
        .pin_tx = SIMULUATE_COM_TX,
        .dma_channel_rx = DMA_MUX_CHANNEL_1,
        .dma_channel_tx = DMA_MUX_CHANNEL_2,
        .size = SIMULATE_RAM_SIZE,
        .data = simulate_data_ram
    };
    usart_init(usart_cfg);
}

int simulate_getData(simulate_data_rx_t *ret)
{
    int16_t size;
    uint8_t temp [29];
    size = usart_read(temp, sizeof(temp), SIMULATE_COM_UART_NUM);
    if (size == sizeof(simulate_data_rx_t) && temp[0] == 0xAA)
    {
        memcpy(ret, temp, size);
        return 0;
    }
    else
    {
        usart_set_cursor(0, 0, SIMULATE_COM_UART_NUM);
        return 1;
    }
}


void test_simulation(void)
{
    uint8_t test [3] = "Hi";
    while(1)
    {
        // do sth
        //usart_write(test, 3, SIMULATE_COM_UART_NUM);
        for (int jj = 0;jj < 25;jj++)
        {   
            // simulate_getData(&data);
            delay_ms(10);
            // just for test
            mixer_calculate((float)jj,0,0,0);
        }
    }
}