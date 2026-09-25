#include "barometer_dps310.h"
#include "math.h"
#include "timer.h"

#include "define_sim.h"
#include "simulate.h"
#include "spi_master.h"


#define NUM_SPI_DPS310                              SPI_NUM_1
#define DPS310_SPI_DMA_READ                         DMA_MUX_CHANNEL_7
#define SPEED_SPI_DPS310                            SPI_SPEED_1MHZ


#define GPIO_CLK_DPS310                             3
#define GPIO_CS_DPS310                              7
#define GPIO_MOSI_DPS310                            5
#define GPIO_MISO_DPS310                            4


#define GPIO_PORT_CS_DPS310                         GPIO_PORT_D
#define GPIO_PORT_CLK_DPS310                        GPIO_PORT_B
#define GPIO_PORT_MOSI_DPS310                       GPIO_PORT_B
#define GPIO_PORT_MISO_DPS310                       GPIO_PORT_B
#define MODE_COM_SPI_DPS310                         SPI_FULL_DUPLEX

// modify if change config

#define SCALE_FACTOR_KP                             1040384
#define SCALE_FACTOR_KT                             524288
#define WAIT_STATE_T                                10


#define BAROMETER_SIZE_READ                         6
#define ADDRESS_READ_DATA_DPS310                    0x00




volatile uint8_t barometer_data_ram [BAROMETER_SIZE_READ * 2];
int8_t flag_read_barometer = 0;
int8_t flag_read_barometer_err = 0;
uint8_t count_read_temp = WAIT_STATE_T;

// base
volatile float baromater_base;
volatile int32_t p_raw_base;

// pressure calib
int32_t c00, c10;
int16_t c11, c20, c21, c30, c01;

// temp calib
// only 12 bit
int16_t c0, c1;

// tempr
int32_t t_raw;

void dps310_callback(spi_flag_cb_t *flag)
{
    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);
}


static inline int dps310_write_reg(uint8_t addr, uint8_t val)
{
    uint8_t temp [2] = {addr, val};
    int check = 0;

    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
    check = spi_write(NUM_SPI_DPS310, 2, temp, SPI_FULL_DUPLEX);
    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);

    return check;
}

static inline int dps310_read_reg(uint8_t addr, uint8_t *ret)
{
    int check = 0;

    // turn bit 7 to 1 to read
    addr |= 0x80;

    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
    check = spi_write(NUM_SPI_DPS310, 1, &addr, SPI_FULL_DUPLEX);
    check = (spi_read(NUM_SPI_DPS310, 1, ret, SPI_FULL_DUPLEX) == 0 && check == 0) ? 0 : 1;
    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);

    return check;
}

// wait = 1 if wait
static inline int dps310_burst_read(uint8_t addr, uint16_t length, uint8_t *ret, uint8_t wait)
{
    // turn bit 7 to 1 to read
    addr |= 0x80;

    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
    spi_write(NUM_SPI_DPS310, 1, &addr, SPI_FULL_DUPLEX);
    spi_read(NUM_SPI_DPS310, length, ret, SPI_FULL_DUPLEX);


    // wait if need
    if (wait == 1)
        while(spi_check_read(NUM_SPI_DPS310) != SPI_TRUE)
        {
            if (spi_check_read(NUM_SPI_DPS310) == SPI_ERR)
                return -1;
        }
            
    return 0;
}


static int barometer_dps310_read_raw_pa(float *ret, int32_t *p_base)
{
#ifdef SIMULATION_ON
    simulate_data_rx_t data_temp;
    if(simulate_getData(&data_temp) != 0) return 1;
    *ret = data_temp.press;
#else
    uint8_t *temp;
    int32_t p_raw;
    uint8_t update_base = 0;
    float p_raw_rc, t_raw_rc;

    // if (i2c_check_read_burst(NUM_I2C_DPS310) != I2C_TRUE)
    //     return -1;

    if (spi_check_read(NUM_SPI_DPS310) != SPI_TRUE)
        return -1;

    if (flag_read_barometer_err)
        return -1;

    if (flag_read_barometer == 0)
        temp = barometer_data_ram + BAROMETER_SIZE_READ;
    else
        temp = barometer_data_ram;

    // handle raw and scale
    p_raw = (int32_t)((temp[0] << 16) | (temp[1] << 8) | temp[2]);
    if (p_raw & 0x00800000)
        p_raw |= 0xFF000000;
    p_raw_rc =(float)p_raw / SCALE_FACTOR_KP;

    if (count_read_temp >= WAIT_STATE_T)
    {
        t_raw = (int32_t)((temp[3] << 16) | (temp[4] << 8) | temp[5]);
        if (t_raw & 0x00800000)
            t_raw |= 0xFF000000;
        
        update_base = 1;
        // reset
        count_read_temp = 0;
    }
    t_raw_rc =(float)t_raw / SCALE_FACTOR_KT;

    // handle calib
    *ret = (float)(c00 + p_raw_rc*(c10 + p_raw_rc*(c20 + p_raw_rc*c30)) + t_raw_rc*c01 + t_raw_rc*p_raw_rc*(c11 + p_raw_rc*c21));

    // update base with temp
    if (update_base == 1)
    {
        baromater_base = (float)(c00 + p_raw_base*(c10 + p_raw_base*(c20 + p_raw_base*c30)) + t_raw_rc*c01 + t_raw_rc*p_raw_base*(c11 + p_raw_base*c21));
    }

    if (p_base != NULL)
        *p_base = p_raw;

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
    check = dps310_burst_read(ADDRESS_READ_DATA_DPS310, BAROMETER_SIZE_READ , ptr, 0);
        

    if (check == 0)
    {
        flag_read_barometer = (flag_read_barometer == 0) ? 1 : 0;
        flag_read_barometer_err = 0;
        count_read_temp++;
    }
    else
    {
        // handle err for read
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
        // read and wait
        if (dps310_burst_read(0x10, 18, data_temp, 1) != 0)
            continue;
        break;
    } 

    

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
    // init with real device here
    // init
    spi_master_config_t cfg = {
        .CPHA = 1,
        .CPOL = 1,
        .speed = SPEED_SPI_DPS310,
        .dma_read = DPS310_SPI_DMA_READ,
        .enable_crc = SPI_FALSE,
        .gpio_clk = GPIO_CLK_DPS310,
        .gpio_cs = GPIO_CS_DPS310,
        .gpio_mosi = GPIO_MOSI_DPS310,
        .gpio_miso = GPIO_MISO_DPS310,
        .port_clk = GPIO_PORT_CLK_DPS310,
        .port_cs = GPIO_PORT_CS_DPS310,
        .port_mosi = GPIO_PORT_MOSI_DPS310,
        .port_miso = GPIO_PORT_MISO_DPS310,
        .spi_num = NUM_SPI_DPS310,
        .mode_com = MODE_COM_SPI_DPS310,
        .spi_irq_en = SPI_TRUE
    };
    spi_master_init(cfg);
    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);
    spi_add_calback(&dps310_callback, NULL, NUM_SPI_DPS310);

    delay_ms(10);


    // first init to dps310
    while (1)
    {
        uint8_t chip_id = 0x00;
        // this config will have
        // 5cm Precision, 64pr sec , 64 oversampling rate for press
        // 4 pr sec, single time for temp

        // reset soft
        if (dps310_write_reg(0x0C, 0x09) != 0)
            continue;
        delay_ms(100);
		
        // enable p-shift
        if (dps310_write_reg(0x09, 0x04) != 0)
            continue;
        delay_ms(100);
        
        // read chip id
        dps310_read_reg(0x0D, &chip_id);
        if (chip_id != 0x10)
            continue;

        // config pressure
        if (dps310_write_reg(0x06, 0x66) != 0)
            continue;
		delay_ms(100);

        // config temp
        if (dps310_write_reg(0x07, 0xA0) != 0)
            continue;
        delay_ms(100);

        // enable background mode
        if (dps310_write_reg(0x08, 0x07) != 0)
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
        
        break;
    }
	
	delay_ms(1000);
    //read 0x10 -> 0x21 to handle calib
    first_handle_calib_coefficient();
    delay_ms(1000);

    float sum = 0;
    int32_t sum_p_raw_base = 0;
    int i = 0;
	count_read_temp = WAIT_STATE_T;
    // handle base
    while (i < 10)
    {
        float temp = 0;
        int32_t temp_p_raw = 0;
        while (send_cmd_read_dps310() != 0);
        while (spi_check_read(NUM_SPI_DPS310) != SPI_TRUE);
        if (barometer_dps310_read_raw_pa(&temp, &temp_p_raw) != 0) continue;

        sum += temp;
        sum_p_raw_base += temp_p_raw;
        i++;
        delay_ms(100);
    }

    p_raw_base = sum_p_raw_base / 10;
    baromater_base = (float)(sum / 10);

#else

    simulate_data_rx_t data_sim;

    float sum = 0;
    int i = 0;
    // handle base
    while (i < 10)
    {
       while (simulate_getData(&data_sim) != 0);
        sum += data_sim.press;
        i++;
        delay_ms(100);
    }
    baromater_base = (float)(sum / 10);
#endif

}


int barometer_dps310_read_2_height(float *ret)
{
    float temp;
    if (barometer_dps310_read_raw_pa(&temp, NULL) != 0) return 1;

    // tranfer to height
    // add minus because z down
    *ret = (float)-44330*(1.0f - powf((temp/baromater_base), 0.1903f));
    return 0;
}