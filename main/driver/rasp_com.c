#include "rasp_com.h"
#include "usart.h"

#define RASP_COM_PORT               GPIO_PORT_D
#define RASP_COM_RX                 6
#define RASP_COM_TX                 5
#define RASP_COM_DMA_RX             DMA_MUX_CHANNEL_1
#define RASP_COM_DMA_TX             DMA_MUX_CHANNEL_2
#define RASP_COM_USART_NUM          USART_2
#define RASP_COM_LENGTH_READ        12
#define RASP_COM_LENGTH_WRITE       12

#define RASP_RAM_SIZE               (RASP_COM_LENGTH_READ*5)

volatile uint8_t rasp_data_ram [RASP_RAM_SIZE];


void rasp_com_init(void)
{
    usart_config_t usart_cfg = {
        .baudrate = USART_BAUDRATE_115200,
        .intr_en = USART_FALSE,
        .msb = USART_FALSE,
        .parity = USART_NONE_PARITY,
        .stop_bit = USART_1_STOP_BITS,
        .usart_num = RASP_COM_USART_NUM,
        .word_length = USART_8_BITS,
        .gpio_port = RASP_COM_PORT,
        .pin_rx = RASP_COM_RX,
        .pin_tx = RASP_COM_TX,
        .dma_channel_rx = DMA_MUX_CHANNEL_1,
        .dma_channel_tx = DMA_MUX_CHANNEL_2,
        .size = RASP_RAM_SIZE,
        .data = rasp_data_ram
    };
    usart_init(usart_cfg);
}

int rasp_2_stm32(uint8_t *ret)
{
    return usart_read(ret, RASP_COM_LENGTH_READ, RASP_COM_USART_NUM);
}

int stm32_2_rasp(uint8_t *buff)
{
    return usart_write(buff, RASP_COM_LENGTH_WRITE, RASP_COM_USART_NUM);
}