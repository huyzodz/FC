#include "gps_be880.h"
#include "usart.h"
#include "i2c_master.h"


#define BE880_COM_RX                                11
#define BE880_COM_TX                                10
#define BE880_COM_PORT                              GPIO_PORT_B

#define BE880_COM_USAT_NUM                          USART_3

#define BE880_DMA_TX                                DMA_MUX_CHANNEL_4
#define BE880_DMA_RX                                DMA_MUX_CHANNEL_5


#define BE880_LENGTH_READ                           1000
// need to change base on data rx
#define BE880_RAM_SIZE                              BE880_LENGTH_READ


#define BE880_I2C_DMA_READ                          DMA_MUX_CHANNEL_6


volatile static uint8_t be880_data_ram[BE880_RAM_SIZE];


void gps_be880_init(be880_type_t cfg)
{
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
            .gpio_scl = GPIO_SCL_BMI270,
            .gpio_sda = GPIO_SDA_BMI270,
            .i2c_mode = MODE_I2C_SPEED_BMI270,
            .i2c_num = NUM_I2C_BMI270,
            .irq_en = I2C_FALSE,
            .port = GPIO_PORT_BMI270
        };
        i2c_init(cfg);

    }
}