#include "test_i2c.h"

#include "timer.h"
#include "imu_bmi270.h"

uint8_t i2c_rx [20];

void test_i2c(void)
{
    // uint8_t data [] = {0x11, 0x23, 0x24, 0x78, 0x29};

    // // init i2c
    // i2c_config_t cfg = {
    //     .analog_filter = I2C_TRUE,
    //     .dma_read = DMA_MUX_CHANNEL_4,
    //     .gpio_scl = 6,
    //     .gpio_sda = 7,
    //     .i2c_mode = I2C_MODE_100KHZ,
    //     .i2c_num = I2C_NUM_1,
    //     .irq_en = I2C_FALSE,
    //     .port = GPIO_PORT_B
    // };
    // i2c_init(cfg);

    // i2c_write_reg(0x42, 0x12, 0x55, I2C_NUM_1);
    // delay_ms(10);
    // i2c_rx[0] = i2c_read_reg(0x42, 0x10, I2C_NUM_1);
    // delay_ms(10);
    // i2c_burst_write(0x42, 0x27, sizeof(data), I2C_NUM_1, data);
	// delay_ms(10);
	// i2c_write_reg(0x42, 0x17, 0x26, I2C_NUM_1);
	// delay_ms(10);
	// i2c_write_reg(0x42, 0x18, 0x27, I2C_NUM_1);
    // delay_ms(10);

    // i2c_burst_read(0x42, 0x17, 4, I2C_NUM_1, DMA_MUX_CHANNEL_4, i2c_rx);
	// delay_ms(10);

    // i2c_burst_read(0x42, 0x17, 4, I2C_NUM_1, DMA_MUX_CHANNEL_4, i2c_rx);
	// delay_ms(10);

    // i2c_burst_read(0x42, 0x17, 4, I2C_NUM_1, DMA_MUX_CHANNEL_4, i2c_rx);
    // //i2c_burst_write(0x42, 0x27, sizeof(bmi270_config_file), I2C_NUM_1, bmi270_config_file);

    bmi270_read(i2c_rx, 1);

}