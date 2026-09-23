#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_


#include "dma.h"
#include "gpio.h"

typedef enum {
    SPI_FALSE = 0,
    SPI_TRUE,

    // only use for error
    SPI_ERR,
} spi_bool_t;

typedef enum {
    SPI_NUM_1 = 0,
    SPI_NUM_4,
    SPI_NUM_2,
    SPI_NUM_3
} spi_num_t;

typedef enum {
    SPI_SPEED_8MHZ = 2,
    SPI_SPEED_4MHZ,
    SPI_SPEED_2MHZ,
    SPI_SPEED_1MHZ
} spi_speed_t;

typedef enum {
    SPI_FULL_DUPLEX = 0,
    SPI_SIMPLEX_TX,
    SPI_SIMPLEX_RX,
    SPI_HALF_DUPLEX
} spi_mode_com_t;


// flag for callback 
typedef struct {
    spi_bool_t transmit_complete;
    spi_bool_t error_crc;
    spi_bool_t mode_fault;
    spi_bool_t overun_err;

    void *data;
} spi_flag_cb_t;

// typedef for callback function
typedef void (*spi_callback_t)(spi_flag_cb_t *arg);

// tydef struct for call back
typedef struct {
    spi_callback_t cb;
    void *data;
} spi_cb_handle_t;


typedef struct {
    gpio_port port;
    uint8_t num;
} spi_gpio_t;


typedef struct {
    spi_num_t spi_num;
    spi_mode_com_t mode_com;
    spi_speed_t speed;

    gpio_port port_mosi;
    gpio_port port_miso;
    gpio_port port_clk;
    gpio_port port_cs;
    uint8_t gpio_mosi;
    uint8_t gpio_miso;
    uint8_t gpio_clk;
    uint8_t gpio_cs;

    dma_mux1_channel_t dma_read;
    // dma_mux1_channel_t dma_write;

    spi_bool_t enable_crc;
    uint8_t crc_size;

    uint8_t CPOL;
    uint8_t CPHA;

    // add more about irq if need
    spi_bool_t spi_irq_en;

} spi_master_config_t;



void spi_master_init(spi_master_config_t cfg);

// add callback function
void spi_add_calback(spi_callback_t cb, void *data, spi_num_t num);

// return 0 if not err
int spi_write(spi_num_t num, uint16_t length, uint8_t *data, spi_mode_com_t mode);

// return 0 if not err
int spi_read(spi_num_t num, uint16_t length, uint8_t *ret, spi_mode_com_t mode);

// return flag check
int spi_check_read(spi_num_t num);





#endif