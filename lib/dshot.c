#include "dshot.h"
#include "stm32h750vbt6.h"
#include "gpio.h"

uint8_t dshot_data[48];

GPIO_TypeDef *DSHOT_GPIO;


/* set AF for gpio A (0 1 2 3)*/
void dshot_gpio_init(gpio_port port, uint8_t pin)
{
    gpio_port Gpio_port = port;
    uint8_t Gpio_pin = pin;


    gpio_handle_t g0 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin
    };
    gpio_handle_t g1 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin + 1
    };
    gpio_handle_t g2 = {
        .alternate = AF0,
        .gpio = Gpio_port,
        .mode = GPIO_OUTPUT_PULL_UP,
        .OutSpeed = very_high_speed,
        .pinNum = Gpio_pin + 2
    };
    gpio_handle_t g3 = {
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

/* timer for tim1 only and use only 4 channel (1 -> 4)*/
void dshot_timer_init(dshot_type_t type)
{
    /* get current clock system */
    SystemCoreClockUpdate();
    uint32_t clock = SystemCoreClock;

    uint16_t clk_timer;
    uint16_t frequency_dshot;

    uint16_t timerARR;

    
    /* enable clock for TIM1 */
    RCC->APB2ENR |= 1;
    /* calculate clk_time */
    if (clock <= 64000000)
        clk_timer = 64;
    else
    {
        /* div by 2 if > 240 */
        if (clock > 240000000)
            clk_timer = clock/(2 * 1000000);
        else
            clk_timer = clock / 1000000;
    }
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
    /* enable request for DMA */
    TIM1->DIER |= (1 << 8);
    /* set UG to use timer */
    TIM1->EGR |= 1;
    /* set count = 0*/
    TIM1->CNT = 0;
}


void dshot_SetAddress_dma(void)
{
    DMA1_Stream0->NDTR = 48;
    DMA1_Stream0->PAR = (uint32_t)&DSHOT_GPIO->BSRR;
    DMA1_Stream0->M0AR = (uint32_t)dshot_data;
}

/* use only with dshot */
void dshot_dma_init(void)
{
    /* very high priority */
    DMA1_Stream0->CR |= (3 << 16);
    /* 8 bit for mem size */
    DMA1_Stream0->CR &= ~(3 << 13);
    /* 32 bit for peripheral size */
    DMA1_Stream0->CR &= ~(3 << 11); //reset bit
    DMA1_Stream0->CR |= (2 << 11);
    /* mem increment */
    DMA1_Stream0->CR |= (1 << 10);
    /* tranfer mem to per */
    DMA1_Stream0->CR &= ~(3 << 6); //reset bit
    DMA1_Stream0->CR |= (1 << 6);
    /* set interrupt when complete */
    DMA1_Stream0->CR |= (1 << 4);
}

/* use to package data */
void dshot_package(void)
{
    
}

/* this will be call in main to init dshot */
void dshot_init(dshot_handle_t cfg)
{
    /*init gpio*/
    uint8_t pin = cfg.pin_start;
    gpio_port port = cfg.gpio_port;
    dshot_type_t dshot_type = cfg.dshot_type;

    DSHOT_GPIO = (GPIO_TypeDef *)(GPIOA_BASE + cfg.gpio_port*0x400UL);


    dshot_gpio_init(port, pin);
    dshot_timer_init(dshot_type);
}