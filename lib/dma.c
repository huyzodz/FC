#include "dma.h"
#include "stm32h750vbt6.h"


#define DMA_CALLBACK_NUM            16

#define DMA_SET_CURSOR(channel)     (((channel) < DMA_MUX_CHANNEL_8) ? (DMA_Stream_TypeDef*)(DMA1_Stream0_BASE + ((channel)*0x18)) : (DMA_Stream_TypeDef*)(DMA2_Stream0_BASE + (((channel) - DMA_MUX_CHANNEL_8)*0x18)))


static dma_callback_handle_t arr_callback[DMA_CALLBACK_NUM];


static inline void dma_irq_callback(dma_mux1_channel_t channel, DMA_TypeDef *dma)
{
    uint8_t check_shift = 0;
    volatile uint32_t *ptr_flag_irq, *ptr_clear_flag_irq;
    dma_flag_callback_t flag = {0};

    /* calculate shift */
    if (channel == DMA_MUX_CHANNEL_0 || channel == DMA_MUX_CHANNEL_4 || channel == DMA_MUX_CHANNEL_8 || channel == DMA_MUX_CHANNEL_12)  
        check_shift = 3;
    else if (channel == DMA_MUX_CHANNEL_1 || channel == DMA_MUX_CHANNEL_5 || channel == DMA_MUX_CHANNEL_9 || channel == DMA_MUX_CHANNEL_13)  
        check_shift = 9;
    else if (channel == DMA_MUX_CHANNEL_2 || channel == DMA_MUX_CHANNEL_6 || channel == DMA_MUX_CHANNEL_10 || channel == DMA_MUX_CHANNEL_14)  
        check_shift = 19;
    else
        check_shift = 25;

    /* set pointer to reg flag irq */
    if (channel < DMA_MUX_CHANNEL_4 || (channel > DMA_MUX_CHANNEL_7 && channel < DMA_MUX_CHANNEL_12))
    {
        ptr_flag_irq = &dma->LISR;
        ptr_clear_flag_irq = &dma->LIFCR;
    }
    else 
    {
        ptr_flag_irq = &dma->HISR;
        ptr_clear_flag_irq = &dma->HIFCR;
    }
        

    if ((*ptr_flag_irq >> check_shift) & 0x01)
    {
        /* error flag */
        flag.error_flag = DMA_TRUE;
    }
    if ((*ptr_flag_irq >> (check_shift + 1)) & 0x01)
    {
        /* half flag */
        flag.half_flag = DMA_TRUE;
    }
    if ((*ptr_flag_irq >> (check_shift + 2)) & 0x01)
    {
        /* complete flag */
        flag.complete_flag = DMA_TRUE;
    }

    /* clear flag */
    *ptr_clear_flag_irq |= (0x7D << (check_shift - 3)); // clear all flag 

    /* add data to flag */
    flag.data = arr_callback[channel].data;

    /* call callback function user define */
    if (arr_callback[channel].cb)
        arr_callback[channel].cb(&flag);
}




void dma_init(dma_config_t cfg)
{   
    dma_mux1_channel_t channel = cfg.dma_channel;
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    dma_priority_t piority = cfg.dma_priority;
    dma_target_size_t mem_size = cfg.dma_mem_size;
    dma_target_size_t per_size = cfg.dma_per_size;
    dma_bool_t mem_increse = cfg.dma_mem_increse;         
    dma_dir_t tranfer_direction = cfg.dma_tranfer_direction;
    dma_bool_t cir_mode = cfg.dma_cir_mode;
    dma_bool_t intr_en = cfg.dma_interupt_enable;
    dma_bool_t complete_tranfer_intr = cfg.complete_tranfer_intr;
    dma_bool_t half_tranfer_intr = cfg.half_tranfer_intr;
    dma_bool_t error_tranfer_intr = cfg.error_tranfer_intr;
    

    /* MUX variable */
    DMAMUX_Channel_TypeDef *dma_mux = (DMAMUX_Channel_TypeDef*)(DMAMUX1_BASE + (channel * 0x04));

    dma_mux_request_t request = cfg.dma_request;
    /* enable clock just only for dma1 and dma2 */
    RCC->AHB1ENR |= 0x03;

    /* set priority */
    dma_stream->CR &= ~(3 << 16);
    dma_stream->CR |= (piority << 16);

    /* set mem size */
    dma_stream->CR &= ~(3 << 13); // reset bit
    dma_stream->CR |=  (mem_size << 13);
    /* set peripheral size */
    dma_stream->CR &= ~(3 << 11); //reset bit
    dma_stream->CR |= (per_size << 11);

    /* mem set increment or not*/
    dma_stream->CR &= ~(1 << 10);
    dma_stream->CR |= (mem_increse << 10);

    /* direction tranfer */
    dma_stream->CR &= ~(3 << 6); //reset bit
    dma_stream->CR |= (tranfer_direction << 6);


    /* cir mode */
    dma_stream->CR &= ~(0x01 << 8); // reset bit
    dma_stream->CR |= (cir_mode << 8);

    /* check intr */
    if (intr_en)
    {
        /* set interrupt when complete */
        dma_stream->CR |= (1 << 4);

        /* enable NVIC dma 1*/
        if (channel == DMA_MUX_CHANNEL_7)
            NVIC_EnableIRQ(DMA1_Stream7_IRQn);
        else if (channel <= DMA_MUX_CHANNEL_6)
            NVIC_EnableIRQ((DMA1_Stream0_IRQn + channel));
        /* enable NVIC dma 2*/
        else if (channel <= DMA_MUX_CHANNEL_12)
            NVIC_EnableIRQ((DMA2_Stream0_IRQn + channel - 8));
        else
            NVIC_EnableIRQ((DMA2_Stream5_IRQn + channel - DMA_MUX_CHANNEL_13));
        
        

        /* set 1 if enable */
        dma_stream->CR |=  (complete_tranfer_intr << 4);
        dma_stream->CR |= (half_tranfer_intr << 3);
        dma_stream->CR |= (error_tranfer_intr << 2);
    }

    /* config MUX */
    /* turn off SE*/
    dma_mux->CCR &= ~(1 << 16);
    /* turn off EGE */
    dma_mux->CCR &= ~(1 << 9);
    /* clear DMA request */
    dma_mux->CCR &= ~(0x7F);
    /* set request */
    dma_mux->CCR |= (request);
}

void dma_start(dma_mux1_channel_t channel)
{
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    dma_stream->CR |= 0x01;
}

void dma_stop(dma_mux1_channel_t channel)
{
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    DMA_TypeDef *dma = (DMA_TypeDef*) ((channel < DMA_MUX_CHANNEL_8) ? DMA1 : DMA2);
    uint8_t check_shift = 0;
    volatile uint32_t *ptr_clear_flag_irq;

    /* stop DMA */
    dma_stream->CR &= ~0x01;
    while(dma_stream->CR & 0x01);

    /* calculate shift */
    if (channel == DMA_MUX_CHANNEL_0 || channel == DMA_MUX_CHANNEL_4 || channel == DMA_MUX_CHANNEL_8 || channel == DMA_MUX_CHANNEL_12)  
        check_shift = 3;
    else if (channel == DMA_MUX_CHANNEL_1 || channel == DMA_MUX_CHANNEL_5 || channel == DMA_MUX_CHANNEL_9 || channel == DMA_MUX_CHANNEL_13)  
        check_shift = 9;
    else if (channel == DMA_MUX_CHANNEL_2 || channel == DMA_MUX_CHANNEL_6 || channel == DMA_MUX_CHANNEL_10 || channel == DMA_MUX_CHANNEL_14)  
        check_shift = 19;
    else
        check_shift = 25;

    /* set pointer to reg flag irq */
    if (channel < DMA_MUX_CHANNEL_4 || (channel > DMA_MUX_CHANNEL_7 && channel < DMA_MUX_CHANNEL_12))
        ptr_clear_flag_irq = &dma->LIFCR;
    else 
        ptr_clear_flag_irq = &dma->HIFCR;



    /* clear flag */
    *ptr_clear_flag_irq |= (0x7D << (check_shift - 3)); // clear all flag
}

uint8_t dma_busy_check(dma_mux1_channel_t channel)
{
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    return (dma_stream->NDTR > 0) ? 1 : 0;
}

void dma_SetAddr(void *src, void *des, uint16_t length, dma_mux1_channel_t channel)
{
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    uint32_t dir = (dma_stream->CR >> 6) & 0x03;
    /* set length tranfer */
    dma_stream->NDTR = length;
    

    /* check which tranfer dir use */
    if (dir == 0x01)
    {
        /* mem to per */
        dma_stream->PAR = (uintptr_t)des;
        dma_stream->M0AR = (uintptr_t)src;
    }
    else if (dir == 0x00)
    {
        /* per to mem */
        dma_stream->PAR = (uintptr_t)src;
        dma_stream->M0AR = (uintptr_t)des;
    }
    else
    {
        /* need to check again */
        /* mem to mem */
        dma_stream->M0AR = (uintptr_t)src;
        dma_stream->M1AR = (uintptr_t)des;
    }
}


void dma_add_callback(dma_callback_handle_t cfg)
{
    arr_callback[cfg.channel].data = cfg.data;

    
    arr_callback[cfg.channel].cb = cfg.cb;
}


uint32_t dma_get_ndtr(dma_mux1_channel_t channel)
{
    DMA_Stream_TypeDef *dma_stream = DMA_SET_CURSOR(channel);
    return dma_stream->NDTR;
}



/*                  call back               */


void DMA1_Stream1_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_1, DMA1);
}


void DMA1_Stream2_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_2, DMA1);
}


void DMA1_Stream3_IRQHandler(void)
{ 
    dma_irq_callback(DMA_MUX_CHANNEL_3, DMA1);
}


void DMA1_Stream4_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_4, DMA1);
}


void DMA1_Stream5_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_5, DMA1);
}


void DMA1_Stream6_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_6, DMA1);
}


void DMA1_Stream7_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_7, DMA1);
}






// DMA2 IRQ
void DMA2_Stream0_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_8, DMA2);
}


void DMA2_Stream1_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_9, DMA2);
}


void DMA2_Stream2_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_10, DMA2);
}


void DMA2_Stream3_IRQHandler(void)
{ 
    dma_irq_callback(DMA_MUX_CHANNEL_11, DMA2);
}


void DMA2_Stream4_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_12, DMA2);
}


void DMA2_Stream5_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_13, DMA2);
}


void DMA2_Stream6_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_14, DMA2);
}


void DMA2_Stream7_IRQHandler(void)
{
    dma_irq_callback(DMA_MUX_CHANNEL_15, DMA2);
}

