#include "barometer_dps310.h"
#include "test.h"
#include "timer.h"
#include "spi_master.h"
#include "gpio.h"

float data_b_t;

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
#define SCALE_FACTOR_KT                             3670016
#define WAIT_STATE_T                                10


#define BAROMETER_SIZE_READ                         6
#define ADDRESS_READ_DATA_DPS310                    0x00

uint8_t chip_id;
uint8_t data_test [20];

void test_cb(spi_flag_cb_t *flag)
{
    gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);
}


void test_spi(void)
{
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
    spi_add_calback(&test_cb, NULL, NUM_SPI_DPS310);

    uint8_t test [2] = {0x0C, 0x09};
	uint8_t test2 [2] = {0x08, 0x05};
    uint8_t rid = 0x8D;
    uint8_t tt = 0x90;
    
    while (1)
    {
        int32_t i = 0;
        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
        spi_write(NUM_SPI_DPS310, 2, test, SPI_FULL_DUPLEX);
        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);

        delay_ms(1000);
		
		gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
        spi_write(NUM_SPI_DPS310, 2, test2, SPI_FULL_DUPLEX);
        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);

        delay_ms(1000);

        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
        spi_write(NUM_SPI_DPS310, 1, &rid, SPI_FULL_DUPLEX);
        spi_read(NUM_SPI_DPS310, 1, &chip_id, SPI_FULL_DUPLEX);
        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 1);
        delay_ms(1000);

        gpio_write(GPIO_PORT_CS_DPS310, GPIO_CS_DPS310, 0);
        spi_write(NUM_SPI_DPS310, 1, &tt, SPI_FULL_DUPLEX);
        spi_read(NUM_SPI_DPS310, 18, data_test, SPI_FULL_DUPLEX);
        while (spi_check_read(NUM_SPI_DPS310) != SPI_TRUE);
    }
    
}