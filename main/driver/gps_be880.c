#include "gps_be880.h"
#include "usart.h"
#include "i2c_master.h"
#include "drone.h"
#include "math.h"
#include "timer.h"
#include "simulate.h"

#define BE880_COM_RX                                11
#define BE880_COM_TX                                10
#define BE880_COM_PORT                              GPIO_PORT_C

#define BE880_COM_USAT_NUM                          USART_3

#define BE880_DMA_TX                                DMA_MUX_CHANNEL_4
#define BE880_DMA_RX                                DMA_MUX_CHANNEL_5


#define BE880_LENGTH_READ                           1000
// need to change base on data rx
#define BE880_RAM_SIZE                              BE880_LENGTH_READ


#define NUM_I2C_BE880                               I2C_NUM_2
#define BE880_I2C_DMA_READ                          DMA_MUX_CHANNEL_6
#define ADDRESS_DEVICE_BE880                        0x0D

#define GPIO_SCL_BE880                              10
#define GPIO_SDA_BE880                              11
#define GPIO_PORT_COMPASS_BE880                     GPIO_PORT_B
#define MODE_I2C_SPEED_BE880                        I2C_MODE_100KHZ



volatile static uint8_t be880_data_ram[BE880_RAM_SIZE];


void gps_be880_init(be880_type_t cfg)
{
#ifdef SIMULATION_ON

// #else
    usart_config_t usart_cfg = {
        .baudrate = USART_BAUDRATE_115200,
        .intr_en = USART_FALSE,
        .msb = USART_FALSE,
        .parity = USART_NONE_PARITY,
        .stop_bit = USART_1_STOP_BITS,
        .usart_num = BE880_COM_USAT_NUM,
        .word_length = USART_8_BITS,
        .gpio_port = BE880_COM_PORT,
        .pin_rx = BE880_COM_RX,
        .pin_tx = BE880_COM_TX,
        .dma_channel_rx = BE880_DMA_RX,
        .dma_channel_tx = BE880_DMA_TX,
        .size = BE880_RAM_SIZE,
        .data = be880_data_ram
    };
    usart_init(usart_cfg);


    // config i2c if need
    if (cfg)
    {
        /* init i2c */
        i2c_config_t cfg = {
            .analog_filter = I2C_TRUE,
            .dma_read = BE880_I2C_DMA_READ,
            .gpio_scl = GPIO_SCL_BE880,
            .gpio_sda = GPIO_SDA_BE880,
            .i2c_mode = MODE_I2C_SPEED_BE880,
            .i2c_num = NUM_I2C_BE880,
            .irq_en = I2C_FALSE,
            .port = GPIO_PORT_COMPASS_BE880
        };
        i2c_init(cfg);

        delay_ms(10);

        // init write for compass
        while (1)
        {
            // set period be880
            if (i2c_write_reg(ADDRESS_DEVICE_BE880, 0x0B, 0x01, NUM_I2C_BE880) != 0)
                continue;
            
            // max power use (ODR = 512, Freq = 200Hz, Scale = 8, Mode = Continue)
            if (i2c_write_reg(ADDRESS_DEVICE_BE880, 0x09, 0x1D, NUM_I2C_BE880) != 0)
                continue;

            break;
        }
    }

#endif
}

int be880_read_compass(compass_data_t *ret)
{
#ifdef SIMULATION_ON
    int check = i2c_burst_read(ADDRESS_DEVICE_BE880, 0x00, 6, NUM_I2C_BE880, BE880_I2C_DMA_READ, temp_compass_i2c);
    
    if (check != 0)
        return check;
	else 
		while (i2c_check_read_burst(NUM_I2C_BE880) != I2C_TRUE);


    ret->x = (int16_t)((temp_compass_i2c[1] << 8) | temp_compass_i2c[0]);
    ret->y = (int16_t)((temp_compass_i2c[3] << 8) | temp_compass_i2c[2]);
    ret->z = (int16_t)((temp_compass_i2c[5] << 8) | temp_compass_i2c[4]);
#else
    uint8_t temp [6];
    int check = i2c_burst_read(ADDRESS_DEVICE_BE880, 0x00, 6, NUM_I2C_BE880, BE880_I2C_DMA_READ, temp);
    

    if (check != 0)
        return check;
	else
		while (i2c_check_read_burst(NUM_I2C_BE880) != I2C_TRUE);

    ret->x = (int16_t)((temp[1] << 8) | temp[0]);
    ret->y = (int16_t)((temp[3] << 8) | temp[2]);
    ret->z = (int16_t)((temp[5] << 8) | temp[4]);
#endif
    
    return 0;
}

int be880_read_compass_2_yaw(float roll, float pitch, float *ret)
{
    float sin_phi = sinf(roll);
    float cos_phi = cosf(roll);
    float sin_theta = sinf(pitch);
    float cos_theta = cosf(pitch);
    compass_data_t data;
    float mag_x_flat, mag_y_flat;

    if (be880_read_compass(&data) != 0) return -1;

    mag_x_flat = data.x * cos_theta + data.y * sin_theta * sin_phi + data.z * sin_theta * cos_phi;
    mag_y_flat = data.y * cos_phi - data.z * sin_phi;

    *ret = atan2f(mag_y_flat, mag_x_flat);

    if (*ret > DRONE_Pi) *ret -= 2.0f * DRONE_Pi;
    else if (*ret < DRONE_Pi) *ret += 2.0f * DRONE_Pi;

    return 0;
}