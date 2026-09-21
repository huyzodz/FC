#include "spi_master.h"
#include "timer.h"
#include "stm32h750vbt6.h"

#define MAX_WAY                     1000000

#define SPI_TURN_SPE_OFF(ptr)       ((ptr)->CR1 &= ~0x01)
#define SPI_TURN_SPE_ON(ptr)        ((ptr)->CR1 |= 0x01)
#define SPI_SET_CSTART(ptr)         ((ptr)->CR1 |= (0x01 << 9))

#define SIMPLEX_TX_SPI(ptr)         ((ptr)->CFG2 &= ~(0x03 << 17), (ptr)->CFG2 |= (0x01 << 17))
#define SIMPLEX_RX_SPI(ptr)         ((ptr)->CFG2 &= ~(0x03 << 17), (ptr)->CFG2 |= (0x02 << 17))


#define HALF_DUPLEX_TX(ptr)         ((ptr)->CR1 |= (0x01 << 11))
#define HALF_DUPLEX_RX(ptr)         ((ptr)->CR1 &= ~(0x01 << 11))


#define SPI_CLEAR_EOT_FLAG(ptr)     ((ptr)->IFCR |= (0x01 << 3))
#define SPI_CLEAR_TXTF_FLAG(ptr)    ((ptr)->IFCR |= (0x01 << 4))
#define SPI_CLEAR_UDR_FLAG(ptr)     ((ptr)->IFCR |= (0x01 << 5))
#define SPI_CLEAR_OVR_FLAG(ptr)     ((ptr)->IFCR |= (0x01 << 6))
#define SPI_CLEAR_CRCE_FLAG(ptr)    ((ptr)->IFCR |= (0x01 << 7))
#define SPI_CLEAR_TIFRE_FLAG(ptr)   ((ptr)->IFCR |= (0x01 << 8))
#define SPI_CLEAR_MODF_FLAG(ptr)    ((ptr)->IFCR |= (0x01 << 9))
#define SPI_CLEAR_TSERF_FLAG(ptr)   ((ptr)->IFCR |= (0x01 << 10))
#define SPI_CLEAR_SUSP_FLAG(ptr)    ((ptr)->IFCR |= (0x01 << 11))

#define SPI_CLEAR_ALL_FLAG(ptr)     ((ptr)->IFCR |= (0x1FF << 3))

#define SPI_ERASE_TSIZE(ptr)        ((ptr)->CR2 &= ~(0xFFFF))
#define SPI_SET_TSIZE(ptr, size)    ((ptr)->CR2 |= (size))

#define SPI_TX_1_BYTE(ptr, data)    (*((__IO uint8_t *)&(ptr)->TXDR) = data)



SPI_TypeDef const *spi_arr_glb [4] = {SPI1, SPI4, SPI2, SPI3};
spi_num_t const spi_num_table [4] = {SPI_NUM_1, SPI_NUM_4, SPI_NUM_2, SPI_NUM_3};
volatile spi_bool_t SPI_CHECK_READ_BURST [4] = {SPI_FALSE, SPI_FALSE, SPI_FALSE, SPI_FALSE};
volatile spi_cb_handle_t spi_cb_table_glb [4];

volatile dma_mux1_channel_t spi_dma_num_glb [4];

 

// function of handle general irq spi
static inline void spi_irq_callback(spi_num_t num)
{
    SPI_TypeDef *ptr = spi_arr_glb[num];
    spi_cb_handle_t handle = spi_cb_table_glb[num];
    spi_flag_cb_t flag = {0};

    // handle irq
    // check transmit done
    if (((ptr->SR >> 3) & 0x01) == 0x01)
        flag.transmit_complete = SPI_TRUE;

    // check error crc
    if (((ptr->SR >> 7) & 0x01) == 0x01)
        flag.error_crc = SPI_TRUE;
    
    // check mode fault
    if (((ptr->SR >> 9) & 0x01) == 0x01)
        flag.mode_fault = SPI_TRUE;

    if (((ptr->SR >> 6) & 0x01) == 0x01)
        flag.overun_err = SPI_TRUE;

    
    flag.data = handle.data;

    // clear flag
    SPI_CLEAR_ALL_FLAG(ptr);


    // call function user define
    if (handle.cb)
        handle.cb(&flag);
}



static inline int wait_tranfer_eot(SPI_TypeDef *ptr)
{
    int32_t i = 0;
    // wait eot or timeout
    while (((ptr->SR >> 3) & 0x01) != 1 && i < MAX_WAY)
    {
        i++;
    }
    if (i >= MAX_WAY)
        return -1;
    return 0;
}

static inline int wait_tranfer_txp(SPI_TypeDef *ptr)
{
    int32_t i = 0;
    // wait txp or timeout
    while (((ptr->SR >> 1) & 0x01) != 1 && i < MAX_WAY)
    {
        i++;
    }
    if (i >= MAX_WAY)
        return -1;
    return 0;
}

static inline int wait_tranfer_txc(SPI_TypeDef *ptr)
{
    int32_t i = 0;
    // wait txc or timeout
    while (((ptr->SR >> 12) & 0x01) != 1 && i < MAX_WAY)
    {
        i++;
    }
    if (i >= MAX_WAY)
        return -1;
    return 0;
}

static inline int wait_tranfer_txtf(SPI_TypeDef *ptr)
{
    int32_t i = 0;
    // wait txtf or timeout
    while (((ptr->SR >> 4) & 0x01) != 1 && i < MAX_WAY)
    {
        i++;
    }
    if (i >= MAX_WAY)
        return -1;
    return 0;
}

static inline int wait_recieve(SPI_TypeDef *ptr)
{
    int32_t i = 0;
    while (((ptr->SR) & 0x01) != 1 && i < MAX_WAY)
    {
        i++;
    }
    if (i >= MAX_WAY)
        return -1;
    return 0;
}

void spi_error_recovery(spi_num_t num)
{
    SPI_TypeDef *ptr = spi_arr_glb[num];
    SPI_TURN_SPE_OFF(ptr);
    // clear all flag
    SPI_CLEAR_ALL_FLAG(ptr);
    
    // fix err for dma if need


    // set tsize
    SPI_ERASE_TSIZE(ptr);
    // TURN_ON_SPI(ptr);
}


static inline void spi_close_transfer(SPI_TypeDef *ptr)
{
    // clear flag
    // clear eot and txtf
    SPI_CLEAR_EOT_FLAG(ptr);
    SPI_CLEAR_TXTF_FLAG(ptr);

    // if rx
    if (((ptr->CR1 >> 11) & 0x01) == 0x01)
    {
        // handle flag for rx

    }

    // turn off spi
    SPI_TURN_SPE_OFF(ptr);
    
    // reset tsize
    SPI_ERASE_TSIZE(ptr);
}

void spi_callback_handle(void *arg)
{
    dma_flag_callback_t *data = (dma_flag_callback_t*)arg;
    spi_num_t *num = (spi_num_t*)data->data;
    SPI_TypeDef *ptr = spi_arr_glb[*num];

    // err
    if (data->error_flag)
    {   
        SPI_CHECK_READ_BURST[*num] = SPI_ERR;
        spi_error_recovery(*num);
    }
        
    
    if (data->complete_flag)
    {
        // clear flag if need
        SPI_CLEAR_ALL_FLAG(ptr);
        SPI_CHECK_READ_BURST[*num] = SPI_TRUE;
    }   

    spi_close_transfer(ptr);
    dma_stop(spi_dma_num_glb[*num]);
}

void spi_dma_init(spi_master_config_t cfg)
{
    spi_num_t num = cfg.spi_num;
    dma_config_t dma_rx = {
        .dma_channel = cfg.dma_read,
        .dma_cir_mode = DMA_FALSE,
        .dma_interupt_enable = DMA_TRUE,
        .complete_tranfer_intr = DMA_TRUE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = BYTE_8_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = BYTE_8_BIT,
        .dma_priority = VERY_HIGH,
        .dma_tranfer_direction = PER_TO_MEM
    };


    /* check i2c that match dma request */
    if (num == SPI_NUM_1)
        dma_rx.dma_request = SPI1_RX_DMA;
    else if (num == SPI_NUM_2)
        dma_rx.dma_request = SPI2_RX_DMA;
    else if (num == SPI_NUM_3)
        dma_rx.dma_request = SPI3_RX_DMA;
    else if (num == SPI_NUM_4)
        dma_rx.dma_request = SPI4_RX_DMA;


    // init dma
    dma_init(dma_rx);

    spi_dma_num_glb[num] = cfg.dma_read;

    dma_callback_handle_t cb = {
        .cb = &spi_callback_handle,
        .channel = cfg.dma_read,
        .data = &spi_num_table[num]
    };

    dma_add_callback(cb);
}

void spi_gpio_init(spi_master_config_t cfg)
{
    alternate_mode alMode = (cfg.spi_num == SPI_NUM_3) ? AF6 : AF5;
    if (cfg.mode_com != SPI_SIMPLEX_RX)
    {
        // mosi
        gpio_config_t mosi = {
            .alternate = alMode,
            .mode = GPIO_ALTERNATE_OUTPUT_PUSH_PULL,
            .gpio = cfg.port_mosi,
            .pinNum = cfg.gpio_mosi,
            .pull = reserved,
            .OutSpeed = very_high_speed
        };
        gpio_init(&mosi);
    }

    // clk
    gpio_config_t clk = {
        .alternate = alMode,
        .mode = GPIO_ALTERNATE_OUTPUT_PUSH_PULL,
        .gpio = cfg.port_clk,
        .pull = reserved,
        .pinNum = cfg.gpio_clk,
        .OutSpeed = very_high_speed
    };

    if (cfg.mode_com != SPI_HALF_DUPLEX && cfg.mode_com != SPI_SIMPLEX_TX)
    {
        // miso
        gpio_config_t miso = {
            .alternate = alMode,
            .mode = GPIO_ALTERNATE_INPUT_PULL_UP,
            .gpio = cfg.port_miso,
            .pull = reserved,
            .pinNum = cfg.gpio_miso,
            .OutSpeed = very_high_speed
        };
        gpio_init(&miso);
    }

    // cs
    gpio_config_t cs = {
        .alternate = AF0,
        .mode = GPIO_OUTPUT_PULL_UP,
        .gpio = cfg.port_cs,
        .pinNum = cfg.gpio_cs,
        .OutSpeed = very_high_speed
    };
    
    
    gpio_init(&clk);
    gpio_init(&cs);

    gpio_write(cfg.port_cs, cfg.gpio_cs, 1);
}


void spi_master_init(spi_master_config_t cfg)
{
    spi_num_t num = cfg.spi_num;
    spi_speed_t speed = cfg.speed;
    //uint32_t clock = SystemCoreClock;
    SPI_TypeDef *ptr = spi_arr_glb[num];

    // enable clock
    if (num <= SPI_NUM_4)
        // enable clock for spi 1 and 4
        RCC->APB2ENR |= (1 << (12 + num));
    else
        // clock for spi 2 3
        RCC->APB1LENR |= (1 << (12 + num));

    // init gpio
    spi_gpio_init(cfg);
    // turn off spi
    ptr->CR1 &= ~(0x01);


    // enable AFCNTR
    ptr->CFG2 |= (1 << 31);

    

    // baurate 
    ptr->CFG1 &= ~(0x07 << 28); //reset
    // set speed
    ptr->CFG1 |= (speed << 28);


    // turn on SSOE
    ptr->CFG2 |= (1 << 29);

    // enable CRC
    if (cfg.enable_crc == SPI_TRUE)
    {
        ptr->CFG1 |= (1 << 22);
        ptr->CFG1 &= ~(0x1F << 16); // reset
        // set crc size
        ptr->CFG1 |= ((cfg.crc_size-1) << 16);
    }
    else 
        ptr->CFG1 &= ~(1 << 22);
    
    // disable dma tx
    ptr->CFG1 &= ~(1 << 15);

    // enable dma rx
    ptr->CFG1 |= (1 << 14);

    // use 8 bit
    ptr->CFG1 &= ~(0x1F); // reset
    ptr->CFG1 |= 0x07; // 8bit

    // CFG2
    ptr->CFG2 &= ~(1 << 29); //dis SSOE
    ptr->CFG2 |= (1 << 30); // en SSOM
    ptr->CFG2 |= (1 << 31); // en for peripheral control gpio

    // first clock is first data
    // MSB first

    // config com
    // reset
    ptr->CFG2 &= ~(0x03 << 17);
    ptr->CFG2 |= (cfg.mode_com << 17);
	
	
	// ss bit set by software
    ptr->CFG2 |= (1 << 26);
    // set SSI to 1
    ptr->CR1 |= (1 << 12);
	
	
	// config CPOL and CPHA
    if (cfg.CPHA)
        ptr->CFG2 |= (1 << 24);
    if (cfg.CPOL)
        ptr->CFG2 |= (1 << 25);
	
	
    ptr->CFG2 |= (1 << 22); // master mode

 

    // init dma for read only
    if (cfg.mode_com != SPI_SIMPLEX_TX)
        spi_dma_init(cfg);


    // if irq enable
    if (cfg.spi_irq_en == SPI_TRUE)
    {
        // en eot/txc/ susp irq
        ptr->IER |= (1 << 3);

        // irq for crc check
        ptr->IER |= (1 << 7);

        // mode fault irq
        ptr->IER |= (1 << 9);

        // overun error irq
        ptr->IER |= (1 << 6);

        // enable NVIC 
        if (num == SPI_NUM_1)
            NVIC_EnableIRQ(SPI1_IRQn);
        if (num == SPI_NUM_2)
            NVIC_EnableIRQ(SPI2_IRQn);    
        if (num == SPI_NUM_3)
            NVIC_EnableIRQ(SPI3_IRQn);
        if (num == SPI_NUM_4)
            NVIC_EnableIRQ(SPI4_IRQn);
    }
}

void spi_add_calback(spi_callback_t cb, void *data, spi_num_t num)
{
    spi_cb_table_glb[num].cb = cb;
    spi_cb_table_glb[num].data = data;
}

int spi_half_write(spi_num_t num, uint16_t length, uint8_t *data)
{
    SPI_TypeDef *ptr = spi_arr_glb[num];
    // wait last flag tranfer
    // wait TXP
    if (wait_tranfer_txp(ptr) == -1)
        goto err_handle;
    
    // check 
    // err when it in simplex rx
    if (((ptr->CFG2 >> 17) & 0x03) == 0x02)
        return -1;

    // switch
    if (((ptr->CR1 >> 11) & 0x01) != 0x01)
    {
        // switch to tx
        SPI_TURN_SPE_OFF(ptr);
        HALF_DUPLEX_TX(ptr);
    }

    // set tsize
    SPI_SET_TSIZE(ptr, length);

    // turn on spi
    SPI_TURN_SPE_ON(ptr);

    // cstart spi
    SPI_SET_CSTART(ptr);

    // write data
    for (int i = 0;i < length;i++)
    {
        SPI_TX_1_BYTE(ptr, data[i]);

        // wait TXP
        if (wait_tranfer_txp(ptr) == -1)
            goto err_handle;
        
        // delay_ms(100);
        
    }
    if (wait_tranfer_eot(ptr) == -1)
        goto err_handle;

    if (wait_tranfer_txc(ptr) == -1)
        goto err_handle;

    // close tranfer
    spi_close_transfer(ptr);

    return 0;

err_handle:
    spi_error_recovery(num);
    return -1;
}

int spi_half_read(spi_num_t num, uint16_t length, uint8_t *ret)
{
    SPI_TypeDef *ptr = spi_arr_glb[num];
    dma_mux1_channel_t dma_num = spi_dma_num_glb[num];
    // check 
    // err when it in simplex tx
    if (((ptr->CFG2 >> 17) & 0x03) == 0x01)
        return -1;

    // check switch to read
    if (((ptr->CR1 >> 11) & 0x01) == 0x01)
    {
        SPI_TURN_SPE_OFF(ptr);
        HALF_DUPLEX_RX(ptr);
    }

    // reset flag
    SPI_CHECK_READ_BURST[num] = SPI_FALSE;

    // set size
    SPI_SET_TSIZE(ptr, length);

    // turn on spi
    SPI_TURN_SPE_ON(ptr);

    // setting dma
    dma_SetAddr((uint8_t*)&ptr->RXDR, (uint8_t*)ret, length, dma_num);

    // start dma
    dma_start(dma_num);

    // cstart
    SPI_SET_CSTART(ptr);

    return 0;
}

int spi_half_check_read(spi_num_t num)
{
    return SPI_CHECK_READ_BURST[num];
}









            /*                  IRQ here                */

void SPI1_IRQHandler(void)
{
    spi_irq_callback(SPI_NUM_1);
}

void SPI2_IRQHandler(void)
{
    spi_irq_callback(SPI_NUM_2);
}

void SPI3_IRQHandler(void)
{
    spi_irq_callback(SPI_NUM_3);
}

void SPI4_IRQHandler(void)
{
    spi_irq_callback(SPI_NUM_4);
}

