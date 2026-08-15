#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>
#include "gpio.h"
#include "dma.h"


typedef enum {
    USART_1 = 0,
    USART_6,
    USART_2,
    USART_3
} usart_num_t;

typedef enum {
    USART_BAUDRATE_9600 = 9600,
    USART_BAUDRATE_19200 = 19200,
    USART_BAUDRATE_38400 = 38400,
    USART_BAUDRATE_57600 = 57600,
    USART_BAUDRATE_115200 = 115200,
    USART_BAUDRATE_230400 = 230400,
    USART_BAUDRATE_460800 = 460800,
    USART_BAUDRATE_921600 = 921600
} usart_baudrate_t;

typedef enum {
    USART_8_BITS = 0,
    USART_9_BITS,
    USART_7_BITS
} usart_word_length_t;

typedef enum {
    USART_PARITY_EVEN = 0,
    USART_OLD_EVEN,
    USART_NONE_PARITY
} usart_parity_t;

typedef enum {
    USART_1_STOP_BITS = 0,
    USART_0_5_STOP_BITS,
    USART_2_STOP_BITS,
    USART_1_5_STOP_BITS
} usart_stop_bit_t;

typedef enum {
    USART_FALSE = 0,
    USART_TRUE
} usart_bool_t;


typedef struct {
    /* read page 77 data sheet */
    uint8_t pin_tx;
    uint8_t pin_rx;
    gpio_port gpio_port;

    usart_num_t usart_num;
    usart_baudrate_t baudrate;
    usart_word_length_t word_length;
    usart_parity_t parity;
    usart_stop_bit_t stop_bit;
    usart_bool_t msb;           /* read rf page 2060 bit 19*/
    

    usart_bool_t intr_en;       /* en intr or not */
    usart_bool_t tranfer_complete_intr; /* en tranfer done intr or not */
    usart_bool_t pe_intr;       /* parity err intr */

    /* set dma channel to use */
    dma_mux1_channel_t dma_channel_rx;
    dma_mux1_channel_t dma_channel_tx;

    /* pointer to data rx*/
    uint8_t *data;
    uint16_t size;

} usart_config_t;


typedef struct {
    uint16_t pRead, pWrite;
    uint16_t size;
    uint8_t cir_flag;
    uint8_t *data;

    dma_mux1_channel_t dma_channel_rx;
    dma_mux1_channel_t dma_channel_tx;
} usart_data_t;



void usart_init(usart_config_t cfg);

/* 
    return -1 when no data
*/
int usart_read(uint8_t *ret, uint16_t length, usart_num_t num);

/*
    reuturn -1 if busy
*/
int usart_write(uint8_t *src, uint16_t length, usart_num_t num);

int usart_set_cursor(int16_t rx, int16_t tx, usart_num_t num);

#endif