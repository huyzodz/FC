#include "clock.h"
#include "stm32h750vbt6.h"

int clock_init(cpu_clock_t clock)
{
    uint8_t clk = 0;

    //RCC->PLLCFGR &= ~(0x07 << 22);
    /* calculate clk pll3 for i2c this clk only 36Mhz*/
    /* select source for i2c 1 -> 4 is pll3 36Mhz*/
    RCC->D3CCIPR &= ~(0x03 << 8); //reset
    RCC->D3CCIPR |= (0x01 << 8); // pll3_r_ck for i2c4
    RCC->D2CCIP2R &= ~(0x03 << 12); //reset
                        // change here
    RCC->D2CCIP2R |= (0x01 << 12); // pll3_r_ck for i2c123

    
    /* config pll3*/
    /* DIVM3 = 32 default after reset */
    /* set DIVN3 = 90*/
    RCC->PLL3DIVR &= ~(0x1FF); // reset
                        // change here
    RCC->PLL3DIVR |= (0x59);
    /* set DIVR3 = 5*/
    RCC->PLL3DIVR &= ~(0x7F << 24); // reset 
    RCC->PLL3DIVR |= (0x04 << 24);


    // use pll2 for spi, this clock only 16Mhz
    // select source for spi 1 2 3 for pll2_p, spi4 for pll2_q
    RCC->D2CCIP1R |= (1 << 12); // pll2_p for spi 123
    RCC->D2CCIP1R |= (1 << 16); // pll2_q for spi 4 5

    // config clock for pll2
    // DIVM2 = 16
    // DIVN2  = 48
    // set divm2 = 16
    RCC->PLLCKSELR &= ~(0x3F << 12); // reset
    RCC->PLLCKSELR |= (0x10 << 12); // set divm2 = 16

    RCC->PLL2DIVR &= ~(0x7F << 16); // reset divq2
    RCC->PLL2DIVR |= (2 << 16); // DIVQ2 = 3
    RCC->PLL2DIVR &= ~(0x7F << 9); // reset divp2
    RCC->PLL2DIVR |= (2 << 9); // DIVP2 = 3
    // set DIVN2 = 48
    RCC->PLL2DIVR &= ~(0x1FF); // reset
    RCC->PLL2DIVR |= 0x2F; 


    // turn on clock pll2 and pll3
    RCC->CR |= (0x01 << 26); // enable clock pll 2
    RCC->CR |= (0x01 << 28); // enable clock pll 3

    // wait ready
    while (((RCC->CR >> 29) & 0x01) != 1 || ((RCC->CR >> 27) & 0x01) != 1);

    if (clock <= CLOCK_64_MHZ)
    {
        /* off clock source to save energy */
        //RCC->PLLCFGR &= ~(0x01FF << 16);
        /* cal and set divide D1CPRE */
        if (clock != CLOCK_64_MHZ)
            clk = 8 + (CLOCK_64_MHZ - clock - 1);
        RCC->D1CFGR |= (clk << 8);
    }
    else
    {
        /* change voltage scale */
        if (clock > CLOCK_280_MHZ && clock <= CLOCK_360_MHZ)
        {
            PWR->D3CR &= ~(0x03 << 14); // reset
            PWR->D3CR |= (0x02 << 14); // scale 2
        }
        else
        {
            PWR->D3CR &= ~(0x03 << 14); // reset
            PWR->D3CR |= (0x03 << 14); // scale 1
        }

        /* select bypass mode */
        PWR->CR3 |= 0x01;

        /* wait for scaling done */
        while (!((PWR->D3CR >> 13) & 1));

            



        clk = 20 + 10*(clock - CLOCK_80_MHZ) - 1;
        RCC->PLLCKSELR &= ~(0x3F << 4); // reset
        RCC->PLLCKSELR |= (8 << 4); // div 8 in DIVM1 
        RCC->PLL1DIVR |= (1 << 9); // div 2 in DIVP1
        RCC->PLL1DIVR &= ~(0x01FF); // reset DIVN1
        RCC->PLL1DIVR |= clk; // Mul DIVN1


        if (clock > CLOCK_240_MHZ)
        {
            RCC->D1CFGR &= ~(0x0F);
            RCC->D1CFGR |= (0x08); /* HPRE / 2 */
        }
            


        if (clock > CLOCK_120_MHZ)
        {
            /* div 2 in D1PPRE */
            RCC->D1CFGR &= ~(0x07 << 4);
            RCC->D1CFGR |= (0x04 << 4); 

            /* div 2 in D2PPRE */
            RCC->D2CFGR &= ~(0x77 << 4);
            RCC->D2CFGR |= (0x44 << 4);

            /* div 2 in D3PPRE */
            RCC->D3CFGR &= ~(0x07 << 4);
            RCC->D3CFGR |= (4 << 4);
        }

        /* setup flash latency */
        FLASH->ACR &= ~0x0F;
        //FLASH->ACR |= 0x06;
        FLASH->ACR |= 0x07;


        /* return -1 if PLL1 is lock */
        if (RCC->CR & (1 << 25))
            return -1;
        /* enable PLL auto after reset */

        /* turn on PLLCLK */
        RCC->CR |= (1 << 24);
        /* wait PLL ready */
        while (!(RCC->CR & (1 << 25)));
        /* chose PLL as sys clk */
        RCC->CFGR |= 0x03; 
        /* wait hw switch to PLL */
        while(((RCC->CFGR >> 3) & 0x07) != 0x03);
    }

    /* no error */ 
    SystemCoreClockUpdate();
    return 0;
}