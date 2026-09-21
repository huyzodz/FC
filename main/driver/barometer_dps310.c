#include "barometer_dps310.h"
#include "math.h"
#include "timer.h"

#include "define_sim.h"
#include "simulate.h"
#include "spi_master.h"

volatile float baromater_base;


#define NUM_SPI_DPS310                              SPI_NUM_1
#define DPS310_SPI_DMA_READ                         DMA_MUX_CHANNEL_7
#define SPEED_SPI_DPS310                            SPI_SPEED_1MHZ


#define GPIO_CLK_DPS310                             3
#define GPIO_CS_DPS310                              4
#define GPIO_MOSI_DPS310                            5


#define GPIO_PORT_CS_DPS310                         GPIO_PORT_B
#define GPIO_PORT_CLK_DPS310                        GPIO_PORT_B
#define GPIO_PORT_MOSI_DPS310                       GPIO_PORT_B
#define MODE_COM_SPI_DPS310                         SPI_HALF_DUPLEX

// modify if change config

#define SCALE_FACTOR_KP                             1040384
#define SCALE_FACTOR_KT                             3670016
#define WAIT_STATE_T                                10


#define BAROMETER_SIZE_READ                         6
#define ADDRESS_READ_DATA_DPS310                    0x00




volatile uint8_t barometer_data_ram [BAROMETER_SIZE_READ * 2];
int8_t flag_read_barometer = 0;
int8_t flag_read_barometer_err = 0;
uint8_t count_read_temp = 0;


// pressure calib
int32_t c00, c10;
int16_t c11, c20, c21, c30, c01;

// temp calib
// only 12 bit
int16_t c0, c1;

// tempr
int32_t t_raw;


static int barometer_dps310_read_raw_pa(float *ret)
{
#ifdef SIMULATION_ON
    simulate_data_rx_t data_temp;
    if(simulate_getData(&data_temp) != 0) return 1;
    *ret = data_temp.press;
#else
    uint8_t *temp;
    int32_t p_raw;
    float p_raw_rc, t_raw_rc;

    // if (i2c_check_read_burst(NUM_I2C_DPS310) != I2C_TRUE)
    //     return -1;

    if (spi_half_check_burst_read(NUM_SPI_DPS310) != SPI_TRUE)
        return -1;

    if (flag_read_barometer_err)
        return -1;

    if (flag_read_barometer == 0)
        temp = barometer_data_ram + BAROMETER_SIZE_READ;
    else
        temp = barometer_data_ram;

    // handle raw and scale
    p_raw = (int32_t)((temp[0] << 16) | (temp[1] << 8) | temp[2]);
    if (p_raw & 0x0800000)
        p_raw |= 0xFF000000;
    p_raw_rc =(float)p_raw / SCALE_FACTOR_KP;

    if (count_read_temp >= WAIT_STATE_T)
    {
        t_raw = (int32_t)((temp[3] << 16) | (temp[4] << 8) | temp[5]);
        if (t_raw & 0x0800000)
            t_raw |= 0xFF000000;
        // reset
        count_read_temp = 0;
    }
    t_raw_rc =(float)t_raw / SCALE_FACTOR_KT;

    // handle calib
    *ret = (float)(c00 + p_raw_rc*(c10 + p_raw_rc*(c20 + p_raw_rc*c30)) + t_raw_rc*c01 + t_raw_rc*p_raw_rc*(c11 + p_raw_rc*c21));

    // if need for show temp
    // Tcomp = c0*0.5f + c1*t_raw_rc;

#endif

    return 0;
}

int send_cmd_read_dps310(void)
{
    int check;
    uint8_t *ptr;
    if (flag_read_barometer == 0)
        ptr = barometer_data_ram;
    else
        ptr = barometer_data_ram + BAROMETER_SIZE_READ;

    // write 0x03 to reg 0x08 to read both temp and press
    //check_write = i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x08, 0x03, NUM_I2C_DPS310);

    //if (check_write != -1)
    //check = i2c_burst_read(ADDRESS_DEVICE_DPS310, 0x00, BAROMETER_SIZE_READ, NUM_I2C_DPS310, DPS310_I2C_DMA_READ, ptr);
    check = spi_half_burst_read_reg(NUM_SPI_DPS310, DPS310_SPI_DMA_READ, ADDRESS_READ_DATA_DPS310, BAROMETER_SIZE_READ, ptr);
        

    if (check == 0)
    {
        // handle err for read
        flag_read_barometer = (flag_read_barometer == 0) ? 1 : 0;
        flag_read_barometer_err = 0;
        count_read_temp++;
    }
    else
    {
        flag_read_barometer_err = 1;
        return -1;
    }
    return 0;
}

void first_handle_calib_coefficient(void)
{
    uint8_t data_temp [18];
    // first read for each c in datasheet
    while (1)
    {
        //if (i2c_burst_read(ADDRESS_DEVICE_DPS310, 0x10, 18, NUM_I2C_DPS310, DPS310_I2C_DMA_READ, data_temp) != 0)
        if (spi_half_burst_read_reg(NUM_SPI_DPS310, DPS310_SPI_DMA_READ, ADDRESS_READ_DATA_DPS310, 18, data_temp) != 0)
            continue;
        break;
    } 

    // wait until read done
    while (spi_half_check_burst_read(NUM_SPI_DPS310) != SPI_TRUE);

    c0 = (int16_t)((data_temp[0] << 4) | ((data_temp[1] >> 4)&0x0F));
    if (c0 & 0x0800) 
        c0 |= 0xF000;

    c1 = (int16_t)(((data_temp[1] & 0x0F) << 8) | data_temp[2]);
    if (c1 & 0x0800)
        c1 |= 0xF000;
    
    c00 = (int32_t)((data_temp[3] << 12) | (data_temp[4] << 4) | ((data_temp[5] & 0xF0) >> 4));
    if (c00 & 0x00080000)
        c00 |= 0xFFF00000;
    
    c10 = (int32_t)(((data_temp[5] & 0x0F) << 16) | (data_temp[6] << 8) | (data_temp[7]));
    if (c10 & 0x00080000)
        c10 |= 0xFFF00000;

    c01 = (int16_t)((data_temp[8] << 8) | data_temp[9]);
    c11 = (int16_t)((data_temp[10] << 8) | data_temp[11]);
    c20 = (int16_t)((data_temp[12] << 8) | data_temp[13]);
    c21 = (int16_t)((data_temp[14] << 8) | data_temp[15]);
    c30 = (int16_t)((data_temp[16] << 8) | data_temp[17]);
}

void barometer_dps310_init(void)
{
#ifndef SIMULATION_ON
    // init with i2c
    // init with real device here
    spi_master_config_t cfg = {
        .CPHA = 0,
        .CPOL = 0,
        .speed = SPEED_SPI_DPS310,
        .dma_read = DPS310_SPI_DMA_READ,
        .enable_crc = SPI_FALSE,
        .gpio_clk = GPIO_CLK_DPS310,
        .gpio_cs = GPIO_CS_DPS310,
        .gpio_mosi = GPIO_MOSI_DPS310,
        .port_clk = GPIO_PORT_CLK_DPS310,
        .port_cs = GPIO_PORT_CS_DPS310,
        .port_mosi = GPIO_PORT_MOSI_DPS310,
        .spi_num = NUM_SPI_DPS310,
        .mode_com = MODE_COM_SPI_DPS310
    };
    spi_master_init(cfg);

    delay_ms(10);


    // first init to dps310
    while (1)
    {
        uint8_t chip_id = 0x00;;
        // this config will have
        // 5cm Precision, 64pr sec , 64 oversampling rate for press
        // 4 pr sec, single time for temp

        // reset soft
        if (spi_half_write_reg(NUM_SPI_DPS310, 0x0C, 0x09) != 0)
            continue;
		
		
        delay_ms(1000);
		continue;
		
        // set interface 3 wire and enable p-shift
        if (spi_half_write_reg(NUM_SPI_DPS310, 0x09, 0x05) != 0)
            continue;
        
        delay_ms(1000);
        
        // read chip id
        spi_half_read_reg(NUM_SPI_DPS310, 0x8D, &chip_id);
        if (chip_id != 0x10)
            continue;

        // config pressure
        if (spi_half_write_reg(NUM_SPI_DPS310, 0x06, 0x66) != 0)
            continue;
		
		delay_ms(1000);
        // config temp
        if (spi_half_write_reg(NUM_SPI_DPS310, 0x07, 0xA0) != 0)
            continue;


        delay_ms(1000);
        // enable background mode
        if (spi_half_write_reg(NUM_SPI_DPS310, 0x08, 0x07) != 0)
            continue;

        // if (i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x0C, 0x09, NUM_I2C_DPS310) != 0)
        //     continue;
        
        // config pressure
        // if (i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x06, 0x66, NUM_I2C_DPS310) != 0)
        //     continue;

        // // config temp
        // if (i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x07, 0xA0, NUM_I2C_DPS310) != 0)
        //     continue;

        // // enable p-shift
        // if (i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x09, 0x04, NUM_I2C_DPS310) != 0)
        //     continue;

        // // enable background mode
        // if (i2c_write_reg(ADDRESS_DEVICE_DPS310, 0x08, 0x07, NUM_I2C_DPS310) != 0)
        //     continue;
        
        // break;
    }

    //read 0x10 -> 0x21 to handle calib
    first_handle_calib_coefficient();
    delay_ms(100);

#endif

    float sum = 0;
    int i = 0;
    // handle base
    while (i < 10)
    {
        float temp = 0;
        while (send_cmd_read_dps310() != 0);
        while (spi_half_check_burst_read(NUM_SPI_DPS310) != SPI_TRUE);
        if (barometer_dps310_read_raw_pa(&temp) != 0) continue;
        sum += temp;
        i++;
        delay_ms(100);
    }
    baromater_base = (float)(sum / 10);
}


int barometer_dps310_read_2_height(float *ret)
{
    float temp;
    if (barometer_dps310_read_raw_pa(&temp) != 0) return 1;

    // tranfer to height
    *ret = (float)44330*(1.0f - powf((temp/baromater_base), 0.1903f));
    return 0;
}