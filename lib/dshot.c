#include "dshot.h"
#include "stm32h750vbt6.h"
#include "dma.h"


#define MIN_DSHOT                   48
#define MAX_DSHOT                   2047

#define BUSY                        0
#define FREE                        1


volatile uint32_t dshot_data [LENGTH_DSHOT_DATA];

GPIO_TypeDef *DSHOT_GPIO;
volatile uint8_t DSHOT_PIN;
/* flag for wait until tranfer done*/
volatile uint8_t FLAG_COMPLETE = FREE;


/* set AF for gpio A (0 1 2 3)*/
void dshot_gpio_init(gpio_port port, uint8_t pin)
{
    gpio_port Gpio_port = port;
    uint8_t Gpio_pin = pin;


    gpio_config_t g0 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin
    };
    gpio_config_t g1 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin + 1
    };
    gpio_config_t g2 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin + 2
    };
    gpio_config_t g3 = {
        .alternate = AF3,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin + 3
    };

    gpio_init(&g0);
    gpio_init(&g1);
    gpio_init(&g2);
    gpio_init(&g3);
}

/* timer for tim1 only and use only 1 CHANNEL 1*/
void dshot_timer_init(dshot_type_t type)
{
    /* get current clock system */
    SystemCoreClockUpdate();
    uint32_t clock = SystemCoreClock;

    uint32_t clk_timer;
    uint32_t frequency_dshot;

    uint32_t timerARR;

    
    /* enable clock for TIM1 */
    RCC->APB2ENR |= 1;
    /* calculate clk_time */
    /* div by 2 if > 240 */
    if (clock > 240000000)
        clk_timer = clock/(2 * 1000000);
    else
        clk_timer = clock / 1000000;
    
    /* scale 100 to not use float*/
    clk_timer *= 100;

    /* 1/ ((1/k) * 10^6 * 1/3) k = 150000/300000/600000/1200000 */
    /* scale 100 for easy */
    if (type == DShot150)
        frequency_dshot = 45;
    else if (type == DShot300)
        frequency_dshot = 90;
    else if (type == DShot600)
        frequency_dshot = 180;
    else
        frequency_dshot = 360;

    /* this will add to TIM_ARR*/
    timerARR = clk_timer/frequency_dshot - 1;

    /* set value to auto reload and interrupt */
    TIM1->ARR = timerARR;
    /* update event when overflow */
    TIM1->CR1 |= (1 << 2);
    /* enable request for DMA (UDE) */
    TIM1->DIER |= (1 << 8);
    /* set UG to use timer */
    TIM1->EGR |= 1;
    /* set count = 0*/
    TIM1->CNT = 0;
}


void dshot_SetAddress_dma(void)
{
    DMA1_Stream0->NDTR = LENGTH_DSHOT_DATA;
    DMA1_Stream0->PAR = (uint32_t)&DSHOT_GPIO->BSRR;
    DMA1_Stream0->M0AR = (uint32_t)dshot_data;
}

/* use only with dshot DMA channel 0*/
void dshot_dma_init(void)
{
    dma_config_t cfg = {
        .complete_tranfer_intr = DMA_TRUE,
        .dma_channel = DMA_MUX_CHANNEL_0,
        .dma_cir_mode = DMA_FALSE,
        .dma_interupt_enable = DMA_TRUE,
        .dma_mem_increse = DMA_TRUE,
        .dma_mem_size = WORD_32_BIT,
        .dma_per_increse = DMA_FALSE,
        .dma_per_size = WORD_32_BIT,
        .dma_priority = VERY_HIGH,
        .dma_request = TIM1_UP,
        .dma_tranfer_direction = MEM_TO_PER,
        .error_tranfer_intr = DMA_FALSE,
        .half_tranfer_intr = DMA_FALSE
    };
    dma_init(cfg);
}

/* stop dshot */
void dshot_stop(void)
{
    /* stop timer */
    TIM1->CR1 &= ~0x01;
    /* stop DMA */
    dma_stop(DMA_MUX_CHANNEL_0);
    /* free flag */
    FLAG_COMPLETE = FREE;
}

/* start dshot */
void dshot_start(void)
{
    /* set addres for dma to tranfer */
    dshot_SetAddress_dma();
    /* start dma */
    dma_start(DMA_MUX_CHANNEL_0);
    /* start timer */
    TIM1->CR1 |= 0x01;
}


void dshot_func_set(uint8_t bit, uint8_t startPin, uint8_t DCNum, uint8_t i)
{
    /* minus 1 because use dc 1 -> 4*/
    DCNum -= 1;

    if (bit == 1)
    {
        dshot_data[i] |= (1 << (startPin+DCNum)); // on 
        dshot_data[i+1] |= (1 << (startPin+DCNum)); // on
        /* this is base on reg BSRR GPIO */ 
        dshot_data[i+2] |= (1 << ((startPin+DCNum)+16)); // off
    }
    else
    {
        dshot_data[i] |= (1 << (startPin+DCNum)); // on 
        dshot_data[i+1] |= (1 << ((startPin+DCNum)+16)); // off
        /* this is base on reg BSRR GPIO */ 
        dshot_data[i+2] |= (1 << ((startPin+DCNum)+16)); // off
    }
}

/* use to package data */
void dshot_package(uint16_t dc1, uint16_t dc2, uint16_t dc3, uint16_t dc4)
{
    uint8_t bitNum = 0;
    uint16_t CRC_Dc1 = 0, CRC_Dc2 = 0, CRC_Dc3 = 0, CRC_Dc4 = 0;

    /* clear package */
    for (uint8_t i = 0;i < LENGTH_DSHOT_DATA;i++)
        dshot_data[i] = 0;


    /* preapare package 11 bit throttle*/
    for (uint8_t i = 0;i < 11*3;i+=3)
    {
        /* check bit */
        uint8_t BitDC1 = 0, BitDC2 = 0, BitDC3 = 0, BitDC4 = 0;
        BitDC1 = ((dc1 >> (10 - bitNum)) & 0x01) ? 1 : 0; 
        BitDC2 = ((dc2 >> (10 - bitNum)) & 0x01) ? 1 : 0; 
        BitDC3 = ((dc3 >> (10 - bitNum)) & 0x01) ? 1 : 0; 
        BitDC4 = ((dc4 >> (10 - bitNum)) & 0x01) ? 1 : 0;

        // set data to array global dshot_data
        dshot_func_set(BitDC1, DSHOT_PIN, 1, i);
        dshot_func_set(BitDC2, DSHOT_PIN, 2, i);
        dshot_func_set(BitDC3, DSHOT_PIN, 3, i);
        dshot_func_set(BitDC4, DSHOT_PIN, 4, i);
        
        // shift 1 bit
        bitNum++;
    }


    /* bit telemetry turn off */
    dshot_func_set(0, DSHOT_PIN, 1, 33);
    dshot_func_set(0, DSHOT_PIN, 2, 33);
    dshot_func_set(0, DSHOT_PIN, 3, 33);
    dshot_func_set(0, DSHOT_PIN, 4, 33);

    /* shift dc to left 1 (add 0 as telemetry) */
    dc1<<=1;
    dc2<<=1;
    dc3<<=1;
    dc4<<=1;

    /* calculate checksum */
    CRC_Dc1 = (dc1 ^ (dc1 >> 4) ^ (dc1 >> 8)) & 0x0F;
    CRC_Dc2 = (dc2 ^ (dc2 >> 4) ^ (dc2 >> 8)) & 0x0F;
    CRC_Dc3 = (dc3 ^ (dc3 >> 4) ^ (dc3 >> 8)) & 0x0F;
    CRC_Dc4 = (dc4 ^ (dc4 >> 4) ^ (dc4 >> 8)) & 0x0F;

    /* add dshot to package */
    bitNum=0;
    for (uint8_t i = 36;i < 48;i+=3)
    {
        /* check bit */
        uint8_t BitCRC1 = 0, BitCRC2 = 0, BitCRC3 = 0, BitCRC4 = 0;
        BitCRC1 = ((CRC_Dc1 >> (3 - bitNum)) & 0x01) ? 1 : 0;
        BitCRC2 = ((CRC_Dc2 >> (3 - bitNum)) & 0x01) ? 1 : 0;
        BitCRC3 = ((CRC_Dc3 >> (3 - bitNum)) & 0x01) ? 1 : 0;
        BitCRC4 = ((CRC_Dc4 >> (3 - bitNum)) & 0x01) ? 1 : 0;

        // set data to array global dshot_data
        dshot_func_set(BitCRC1, DSHOT_PIN, 1, i);
        dshot_func_set(BitCRC2, DSHOT_PIN, 2, i);
        dshot_func_set(BitCRC3, DSHOT_PIN, 3, i);
        dshot_func_set(BitCRC4, DSHOT_PIN, 4, i);
        bitNum++;
    }
        
    
}

/* this will be call in main to init dshot */
int dshot_init(dshot_handle_t cfg)
{
    /*init gpio*/
    uint8_t pin = cfg.pin_start;
    gpio_port port = cfg.gpio_port;
    dshot_type_t dshot_type = cfg.dshot_type;

    /* ERROR */
    if (pin > 12)
        return -1;

    DSHOT_GPIO = (GPIO_TypeDef *)(GPIOA_BASE + cfg.gpio_port*0x400UL);
    DSHOT_PIN = cfg.pin_start;

    dshot_gpio_init(port, pin);
    dshot_timer_init(dshot_type);
    dshot_dma_init();

    
    return 0;
}

uint16_t check_err(uint16_t dc)
{
    if (dc < MIN_DSHOT) return MIN_DSHOT;
    if (dc > MAX_DSHOT) return MAX_DSHOT;
    return dc;
}


void dshot_write(uint16_t dc1, uint16_t dc2, uint16_t dc3, uint16_t dc4)
{
    /* check err or overfolw */
    dc1 = check_err(dc1);
    dc2 = check_err(dc2);
    dc3 = check_err(dc3);
    dc4 = check_err(dc4);


    /* wait until last tranfer done or could do sth else if need */
    while (FLAG_COMPLETE == BUSY);
    /* set busy */
    FLAG_COMPLETE = BUSY;
    dshot_package(dc1,dc2,dc3,dc4);
    dshot_start();
}











/************           IRQ       ************ */


/* DMA interrupt when complete tranfer */
void DMA1_Stream0_IRQHandler(void)
{
    if ((DMA1->LISR >> 5) & 0x01)
    {
        /* clear flag */
        DMA1->LIFCR |= (1 << 5);
        /* turn off dshot */
        dshot_stop();
    }
}
