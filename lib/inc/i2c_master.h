#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_

#include "gpio.h"
#include "dma.h"

typedef enum {
    I2C_FALSE = 0,
    I2C_TRUE
} i2c_bool_t;


typedef enum {
    I2C_NUM_1 = 0,
    I2C_NUM_2,
    I2C_NUM_3
    //I2C_NUM_4
} i2c_num_t;

typedef enum {
    I2C_MODE_100KHZ = 0,
    I2C_MODE_400KHZ,
    I2C_MODE_1000KHZ
} i2c_mode_t;


typedef struct {
    i2c_num_t i2c_num;
    i2c_mode_t i2c_mode;
    i2c_bool_t analog_filter;
    // i2c_bool_t digital_filter;

    gpio_port port;
    uint8_t gpio_scl;
    uint8_t gpio_sda;

    /* set dma for use*/
    dma_mux1_channel_t dma_read;


    i2c_bool_t irq_en;
    i2c_bool_t irq_err_en;
    i2c_bool_t irq_tranfer_complete_en;
    i2c_bool_t irq_rx_en;
    i2c_bool_t irq_tx_en;
    i2c_bool_t irq_stop_dectect_en;
    i2c_bool_t irq_nackie_en;
} i2c_config_t;


typedef enum {
    MODE_WRITE = 0,
    MODE_READ
} i2c_mode_tranfer_t;


// set address of device
int i2c_set_address(uint8_t addr, i2c_num_t i2c_num);

/* just support only AF4*/
void i2c_init(i2c_config_t cfg);

// start, need to stop before call
// set num_byte_tranfe to 256 if byte tranfer > 255 to enable reload
// max each tranfer is 255 => need to reload nbyte manualy
int i2c_start(i2c_num_t i2c_num, uint16_t num_byte_tranfer, i2c_mode_tranfer_t mode, uint8_t addr_dev, i2c_bool_t auto_end);
// restart i2c not need to stop before call
int i2c_restart(i2c_num_t i2c_num, uint8_t num_byte_tranfer, i2c_mode_tranfer_t mode, i2c_bool_t auto_end);

void i2c_stop_tranfer(i2c_num_t i2c_num);


/* return 0 if success else return -1 */
int i2c_write_byte(uint8_t val, i2c_num_t i2c_num);

/* return 0 if success else return -1 */
int i2c_read_byte(uint8_t *ret, i2c_num_t i2c_num);

int i2c_write_reg(uint8_t addr_dev, uint8_t addr, uint8_t val, i2c_num_t num);
uint8_t i2c_read_reg(uint8_t addr_dev, uint8_t addr, i2c_num_t num);

/*
    max length is 255 byte
*/
/*
    paramater
    1: address of dev
    2: address of reg of dev
    3: length data to read (byte)
    4: channel i2c to read
    5: channel dma has applied to cfg
    6: arr of return data

*/
int i2c_burst_read(uint8_t addr_dev, uint8_t addr, uint16_t length, i2c_num_t num, dma_mux1_channel_t dma_channel, uint8_t *ret);
i2c_bool_t i2c_check_read_burst();

int i2c_burst_write(uint8_t addr_dev, uint8_t addr, uint16_t length, i2c_num_t num, uint8_t *buff);


#endif
