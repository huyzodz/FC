#include "stm32h750vbt6.h"
#include "usart.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/* use only in this file */
static volatile usart_data_t *usart_data_glb_rx [4] = {NULL};


static USART_TypeDef *const usart_table [] = {USART1, USART6, USART2, USART3}; 

void usart_callback_dma_rx(void *arg)
{
    dma_flag_callback_t *flag = (dma_flag_callback_t*)arg;
    uint8_t *num = (uint8_t*)flag->data;
    *num = 1;
}



void usart_dma_init(dma_mux1_channel_t channel_rx, dma_mux1_channel_t channel_tx, usart_num_t num)
{
    /* channel for rx */
    dma_config_t cfg_rx = {
        .dma_channel = channel_rx,
        .dma_cir_mode = DMA_TRUE,
        .dma_interupt_enable = DMA_TRUE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = BYTE_8_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = BYTE_8_BIT,
        .dma_priority = MEDIUM,
        .dma_tranfer_direction = PER_TO_MEM,
        .complete_tranfer_intr = DMA_TRUE
    };

    /* channel for tx */
    dma_config_t cfg_tx = {
        .dma_channel = channel_tx,
        .dma_cir_mode = DMA_FALSE,
        .dma_interupt_enable = DMA_FALSE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = BYTE_8_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = BYTE_8_BIT,
        .dma_priority = MEDIUM,
        .dma_tranfer_direction = MEM_TO_PER,
    };

    /* select request */
    if (num == USART_1)
    {
        /* usart 1*/
        cfg_rx.dma_request = USART1_RX_DMA;
        cfg_tx.dma_request = USART1_TX_DMA;
    }
    else if (num == USART_6)
    {
        /* usart 6 */
        cfg_rx.dma_request = USART6_RX_DMA;
        cfg_tx.dma_request = USART6_TX_DMA;
    }
    else if (num == USART_2)
    {
        /* usart 2 */
        cfg_rx.dma_request = USART2_RX_DMA;
        cfg_tx.dma_request = USART2_TX_DMA;
    }
    else
    {
        /* usart 3 */
        cfg_rx.dma_request = USART3_RX_DMA;
        cfg_tx.dma_request = USART3_TX_DMA;
    }

    dma_init(cfg_rx);
    dma_init(cfg_tx);
}

void usart_gpio_init(uint8_t pin_tx, uint8_t pin_rx, gpio_port port)
{
    gpio_config_t tx = {
        .alternate = AF7,
        .gpio = port,
        .mode = GPIO_ALTERNATE_OUTPUT_PUSH_PULL,
        .OutSpeed = high_speed,
        .pinNum = pin_tx
    };
    gpio_config_t rx = {
        .alternate = AF7,
        .gpio = port,
        .mode = GPIO_ALTERNATE_INPUT_PULL_UP,
        .pinNum = pin_rx
    };
    
    /* exception */
    if (port == GPIO_PORT_B && pin_tx == 14)
    {
        tx.alternate = AF4;
        rx.alternate = AF4;
    }

    gpio_init(&tx);
    gpio_init(&rx);
}


void usart_init(usart_config_t cfg)
{
    usart_num_t num = cfg.usart_num;
    usart_parity_t parity = cfg.parity;
    usart_baudrate_t baudrate = cfg.baudrate;
    usart_stop_bit_t stop_bit = cfg.stop_bit;
    usart_word_length_t word_length = cfg.word_length;
    usart_bool_t msb = cfg.msb;

    uint8_t pin_tx = cfg.pin_tx;
    uint8_t pin_rx = cfg.pin_rx;
    gpio_port port = cfg.gpio_port;

    usart_bool_t intr_en = cfg.intr_en;
    usart_bool_t tranfer_complete_intr = cfg.tranfer_complete_intr;
    usart_bool_t pe_intr = cfg.pe_intr;

    dma_mux1_channel_t dma_rx = cfg.dma_channel_rx;
    dma_mux1_channel_t dma_tx = cfg.dma_channel_tx;

    uint8_t *data_rx = cfg.data;
    uint16_t size_data_rx = cfg.size;


    uint32_t temp;

    /* init gpio */
    usart_gpio_init(pin_tx, pin_rx, port);

    /* get clk */
    SystemCoreClockUpdate();
    uint32_t clock = SystemCoreClock;
    /* calculate clk_time */
    if (clock > 120000000)
        clock = clock/2;
    if (clock > 240000000)
        clock = clock/2;

    /* set ptr */
    USART_TypeDef *usart = usart_table[num];


    /* enable clk */
    if (num < USART_2)
        RCC->APB2ENR |= (1 << (4 + num - USART_1));
    else
        RCC->APB1LENR |= (1 << (17 + num - USART_2));


    /* set word length */
    usart->CR1 &= ~(0x01 << 28); // reset bit
    usart->CR1 &= ~(0x01 << 12); // reset bit
    /* M1 (M[1;0]) */
    usart->CR1 |= (((word_length>>1) & 0x01) << 28);
    /* M0 */
    usart->CR1 |= ((word_length & 0x01) << 12);

    /* parity control */
    if (parity == USART_NONE_PARITY)
        usart->CR1 &= ~(0x01 << 10); /* dont use parity */
    else
    {
        usart->CR1 |= (0x01 << 10); /* use parity */
        usart->CR1 |= (parity << 9); /* set parity */
    }

    /* intr cfg */
    if (intr_en)
    {
        usart->CR1 |= (tranfer_complete_intr << 6);
        usart->CR1 |= (pe_intr << 8);

        if (num == USART_1)
            NVIC_EnableIRQ(USART1_IRQn);
        else if (num == USART_2)
            NVIC_EnableIRQ(USART2_IRQn);
        else if (num == USART_3)
            NVIC_EnableIRQ(USART3_IRQn);
        else
            NVIC_EnableIRQ(USART6_IRQn);
    }

    /* enable both TE and RE */
    usart->CR1 |= (0x03 << 2);

    /* set msb */
    usart->CR2 |= (msb << 19);

    /* apply baudrate */
    temp = ((clock * 10)/baudrate)%10;
    if (temp >= 5)
        usart->BRR = ((clock/baudrate)+1);
    else
        usart->BRR = (clock/baudrate);
    
    /* stop bits*/
    usart->CR2 &= ~(0x03 << 12); //reset
    usart->CR2 |= (stop_bit << 12); //reset
    

    /* dma enable tranfer enable both tx and rx dma */
    usart->CR3 |= (0x03 << 6);


    /* init struct for use in this file */
    usart_data_glb_rx[num] = (usart_data_t*)malloc(sizeof(usart_data_t));
    usart_data_glb_rx[num]->cir_flag = 0;
    usart_data_glb_rx[num]->data = data_rx;
    usart_data_glb_rx[num]->pRead = 0;
    usart_data_glb_rx[num]->pWrite = 0;
    usart_data_glb_rx[num]->size = size_data_rx;
    usart_data_glb_rx[num]->dma_channel_rx = dma_rx;
    usart_data_glb_rx[num]->dma_channel_tx = dma_tx;

    /* add callback dma */
    dma_callback_handle_t dma_callback_cfg = {
        .cb = &usart_callback_dma_rx,
        .channel = dma_rx,
        .data = &usart_data_glb_rx[num]->cir_flag
    };


    /* apply dma */
    usart_dma_init(dma_rx, dma_tx, num);
    /* set address dma */
    dma_SetAddr(&usart->RDR, data_rx, size_data_rx, dma_rx);
    /* set callback */
    dma_add_callback(dma_callback_cfg);
    /* start dma */
    dma_start(dma_rx);



    /* enable usart (UE)*/
    usart->CR1 |= 0x01;
}

int get_size(uint16_t pRead, uint16_t pWrite, uint8_t cir_flag, uint16_t size)
{
    if (cir_flag)
        return (pRead < pWrite) ? (pWrite - pRead) : (size - pRead + pWrite);
    
    return (pWrite - pRead);
}


int usart_read(uint8_t *ret, uint16_t length, usart_num_t num)
{
    /* set ptr */
    USART_TypeDef *usart = usart_table[num];

    uint8_t cir_flag = usart_data_glb_rx[num]->cir_flag;
    uint8_t *data_rx = usart_data_glb_rx[num]->data;
    uint16_t pWrite = usart_data_glb_rx[num]->pWrite;
    uint16_t pRead = usart_data_glb_rx[num]->pRead;
    uint16_t size = usart_data_glb_rx[num]->size;
    dma_mux1_channel_t channel = usart_data_glb_rx[num]->dma_channel_rx;


    /* set dma */
    DMA_Stream_TypeDef *dma = (DMA_Stream_TypeDef*)(DMA1_Stream0_BASE + (channel*0x18));

    uint16_t size_read;

    /* check and handle pWrite */
    if (cir_flag == 1)
    {
        /* flag = 1 when dma tranfer reach end and back to begin */
        if (pRead < (size - dma->NDTR))
        {   
            /* should change to plus lenght*/
            //pRead = size - dma->NDTR;
            /* use this to tranfer if data is block */
            pRead += length;
        }
            

        pWrite = size;
    }
	else
		pWrite = size - dma->NDTR;

    /* return -1 if no data */
    if (get_size(pRead, pWrite, cir_flag, size) < length)
        return -1;

    /* condition when cir */
    size_read = length;


    

    for (int i = 0;i < size_read;i++)
    {
        ret[i] = data_rx[pRead];
        pRead++;
		if (pRead >= size)
        {
            pRead = 0;
            usart_data_glb_rx[num]->cir_flag = 0;
        }
    }

    /* update */
    usart_data_glb_rx[num]->pWrite = pWrite ;
    usart_data_glb_rx[num]->pRead = pRead;
    

    return 0;
}

int usart_write(uint8_t *src, uint16_t length, usart_num_t num)
{
    /* set ptr */
    USART_TypeDef *usart = usart_table[num];
    dma_mux1_channel_t channel = usart_data_glb_rx[num]->dma_channel_tx;

    if (dma_busy_check(channel))
        return -1;
    dma_stop(channel);

    dma_SetAddr(src, &usart->TDR, length, channel);
    dma_start(channel);
    return 0;
}


int usart_set_cursor(int16_t rx, int16_t tx, usart_num_t num)
{
    /* set ptr */
    USART_TypeDef *usart = usart_table[num];

    if (rx != -1)
    {
        if (rx >= 0 && rx < usart_data_glb_rx[num]->size)
            usart_data_glb_rx[num]->pRead = rx;
        else 
            return -1;
    }

    if (tx != -1)
    {
        if (tx >= 0 && tx < usart_data_glb_rx[num]->size)
            usart_data_glb_rx[num]->pRead = tx;
        else 
            return -1;
    }

    return 0;
}


/*                          IRQ                         */
