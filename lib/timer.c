#include "timer.h"
#include "stm32h750vbt6.h"


#define DELAY_US(time, ptime)      \
do {                               \
    /*reset cnt to 0*/                  \
    (ptime)->CNT = 0;              \    
    /* clear flag UIF*/                 \
    (ptime)->SR &= ~0x01;          \
    /* set time to ARR */               \
    (ptime)->ARR = (time);         \
    /* enable CEN*/                     \
    (ptime)->CR1 |= 0x01;          \
    /*wait until counter reach to ARR*/ \
    while (!((ptime)->SR & 0x01)); \
    /* turn off counter */              \
    (ptime)->CR1 &= ~0x01;         \
} while(0)                               

 
void basic_init(timer_general_type_t type)
{
    TIM_TypeDef *timer = (TIM_TypeDef*)(TIM2_BASE + type*0x400);

    uint32_t clk_timer;
    uint32_t clock;

    // enable clk
    RCC->APB1LENR |= (0x01 << type);

    /* get current clock system */
    SystemCoreClockUpdate();
    clock = SystemCoreClock;
    
    /* calculate clk_time */
    /* div by 2 if > 240 */
    if (clock > 240000000)
        clk_timer = clock/2;
    else
        clk_timer = clock;
    
    // set 1 Mhz for frequency counter
    timer->PSC = (clk_timer / 1000000) - 1;

    // enable UG
    timer->EGR |= 0x01;
}

void timer_init()
{
    // init for delay
    basic_init(TIMER2);

    // init for measure
    basic_init(TIMER5);
    // init config for measure
    TIM5->CNT = 0;
    TIM5->SR &= ~0x01;
    TIM5->ARR = 0xFFFFFFFF;
    TIM5->CNT = 0;
    // start
    TIM5->CR1 |= 0x01;
}


void delay_us(uint32_t time)
{
    DELAY_US(time, TIM2);
}

void delay_ms(uint32_t time)
{
    while (time > 0)
    {
        DELAY_US(1000, TIM2);
        time--;
    }
}

uint32_t GET_CURRENT_US()
{
    return TIM5->CNT;
}

