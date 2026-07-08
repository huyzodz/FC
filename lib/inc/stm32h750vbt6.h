#ifndef _STM32H750VBT6_H_
#define _STM32H750VBT6_H_

#include <stdint.h>
#include "system_stm32h7xx.h"

#define __IO                volatile            /* read/write permission*/
#define __O                 volatile            /* write only */
#define __I                 volatile const      /* read only */

/*

                            struct define
*/

/* FLASH_TypdeDef */
typedef struct {
    __IO uint32_t ACR;             /*!< FLASH access control register,                            Address offset: 0x00  */
    __IO uint32_t KEYR1;           /*!< Flash Key Register for bank1,                             Address offset: 0x04  */
    __IO uint32_t OPTKEYR;         /*!< Flash Option Key Register,                                Address offset: 0x08  */
    __IO uint32_t CR1;             /*!< Flash Control Register for bank1,                         Address offset: 0x0C  */
    __IO uint32_t SR1;             /*!< Flash Status Register for bank1,                          Address offset: 0x10  */
    __IO uint32_t CCR1;            /*!< Flash Control Register for bank1,                         Address offset: 0x14  */
    __IO uint32_t OPTCR;           /*!< Flash Option Control Register,                            Address offset: 0x18  */
    __IO uint32_t OPTSR_CUR;       /*!< Flash Option Status Current Register,                     Address offset: 0x1C  */
    __IO uint32_t OPTSR_PRG;       /*!< Flash Option Status to Program Register,                  Address offset: 0x20  */
    __IO uint32_t OPTCCR;          /*!< Flash Option Clear Control Register,                      Address offset: 0x24  */
    __IO uint32_t PRAR_CUR1;       /*!< Flash Current Protection Address Register for bank1,      Address offset: 0x28  */
    __IO uint32_t PRAR_PRG1;       /*!< Flash Protection Address to Program Register for bank1,   Address offset: 0x2C  */
    __IO uint32_t SCAR_CUR1;       /*!< Flash Current Secure Address Register for bank1,          Address offset: 0x30  */
    __IO uint32_t SCAR_PRG1;       /*!< Flash Secure Address to Program Register for bank1,       Address offset: 0x34  */
    __IO uint32_t WPSN_CUR1;       /*!< Flash Current Write Protection Register on bank1,         Address offset: 0x38  */
    __IO uint32_t WPSN_PRG1;       /*!< Flash Write Protection to Program Register on bank1,      Address offset: 0x3C  */
    __IO uint32_t BOOT_CUR;        /*!< Flash Current Boot Address for Pelican Core Register,     Address offset: 0x40  */
    __IO uint32_t BOOT_PRG;        /*!< Flash Boot Address to Program for Pelican Core Register,  Address offset: 0x44  */
    uint32_t      RESERVED0[2];    /*!< Reserved, 0x48 to 0x4C                                                          */
    __IO uint32_t CRCCR1;          /*!< Flash CRC Control register For Bank1 Register ,           Address offset: 0x50  */
    __IO uint32_t CRCSADD1;        /*!< Flash CRC Start Address Register for Bank1 ,              Address offset: 0x54  */
    __IO uint32_t CRCEADD1;        /*!< Flash CRC End Address Register for Bank1 ,                Address offset: 0x58  */
    __IO uint32_t CRCDATA;         /*!< Flash CRC Data Register for Bank1 ,                       Address offset: 0x5C  */
    __IO uint32_t ECC_FA1;         /*!< Flash ECC Fail Address For Bank1 Register ,               Address offset: 0x60  */
    uint32_t      RESERVED1[40];   /*!< Reserved, 0x64 to 0x100                                                         */
    __IO uint32_t KEYR2;           /*!< Flash Key Register for bank2,                             Address offset: 0x104 */
    uint32_t      RESERVED2;       /*!< Reserved, 0x108                                                                 */
    __IO uint32_t CR2;             /*!< Flash Control Register for bank2,                         Address offset: 0x10C */
    __IO uint32_t SR2;             /*!< Flash Status Register for bank2,                          Address offset: 0x110 */
    __IO uint32_t CCR2;            /*!< Flash Status Register for bank2,                          Address offset: 0x114 */
    uint32_t      RESERVED3[4];    /*!< Reserved, 0x118 to 0x124                                                        */
    __IO uint32_t PRAR_CUR2;       /*!< Flash Current Protection Address Register for bank2,      Address offset: 0x128 */
    __IO uint32_t PRAR_PRG2;       /*!< Flash Protection Address to Program Register for bank2,   Address offset: 0x12C */
    __IO uint32_t SCAR_CUR2;       /*!< Flash Current Secure Address Register for bank2,          Address offset: 0x130 */
    __IO uint32_t SCAR_PRG2;       /*!< Flash Secure Address Register for bank2,                  Address offset: 0x134 */
    __IO uint32_t WPSN_CUR2;       /*!< Flash Current Write Protection Register on bank2,         Address offset: 0x138 */
    __IO uint32_t WPSN_PRG2;       /*!< Flash Write Protection to Program Register on bank2,      Address offset: 0x13C */
    uint32_t      RESERVED4[4];    /*!< Reserved, 0x140 to 0x14C                                                        */
    __IO uint32_t CRCCR2;          /*!< Flash CRC Control register For Bank2 Register ,           Address offset: 0x150 */
    __IO uint32_t CRCSADD2;        /*!< Flash CRC Start Address Register for Bank2 ,              Address offset: 0x154 */
    __IO uint32_t CRCEADD2;        /*!< Flash CRC End Address Register for Bank2 ,                Address offset: 0x158 */
    __IO uint32_t CRCDATA2;        /*!< Flash CRC Data Register for Bank2 ,                       Address offset: 0x15C */
    __IO uint32_t ECC_FA2;         /*!< Flash ECC Fail Address For Bank2 Register ,               Address offset: 0x160 */
} FLASH_TypeDef;



/* PWR_TypeDef */
typedef struct {
    __IO uint32_t CR1;       /*!< PWR power control register 1,            Address offset: 0x00 */
    __IO uint32_t CSR1;      /*!< PWR power control status register 1,     Address offset: 0x04 */
    __IO uint32_t CR2;       /*!< PWR power control register 2,            Address offset: 0x08 */
    __IO uint32_t CR3;       /*!< PWR power control register 3,            Address offset: 0x0C */
    __IO uint32_t CPUCR;     /*!< PWR CPU control register,                Address offset: 0x10 */
        uint32_t RESERVED0; /*!< Reserved,                                Address offset: 0x14 */
    __IO uint32_t D3CR;      /*!< PWR D3 domain control register,          Address offset: 0x18 */
        uint32_t RESERVED1; /*!< Reserved,                                Address offset: 0x1C */
    __IO uint32_t WKUPCR;    /*!< PWR wakeup clear register,               Address offset: 0x20 */
    __IO uint32_t WKUPFR;    /*!< PWR wakeup flag register,                Address offset: 0x24 */
    __IO uint32_t WKUPEPR;   /*!< PWR wakeup enable and polarity register, Address offset: 0x28 */
} PWR_TypeDef;



/* RCC_TypeDef */
typedef struct {
    __IO uint32_t CR;             /*!< RCC clock control register,                                              Address offset: 0x00  */
    __IO uint32_t HSICFGR;        /*!< HSI Clock Calibration Register,                                          Address offset: 0x04  */
    __IO uint32_t CRRCR;          /*!< Clock Recovery RC  Register,                                             Address offset: 0x08  */
    __IO uint32_t CSICFGR;        /*!< CSI Clock Calibration Register,                                          Address offset: 0x0C  */
    __IO uint32_t CFGR;           /*!< RCC clock configuration register,                                        Address offset: 0x10  */
    uint32_t     RESERVED1;       /*!< Reserved,                                                                Address offset: 0x14  */
    __IO uint32_t D1CFGR;         /*!< RCC Domain 1 configuration register,                                     Address offset: 0x18  */
    __IO uint32_t D2CFGR;         /*!< RCC Domain 2 configuration register,                                     Address offset: 0x1C  */
    __IO uint32_t D3CFGR;         /*!< RCC Domain 3 configuration register,                                     Address offset: 0x20  */
    uint32_t     RESERVED2;       /*!< Reserved,                                                                Address offset: 0x24  */
    __IO uint32_t PLLCKSELR;      /*!< RCC PLLs Clock Source Selection Register,                                Address offset: 0x28  */
    __IO uint32_t PLLCFGR;        /*!< RCC PLLs  Configuration Register,                                        Address offset: 0x2C  */
    __IO uint32_t PLL1DIVR;       /*!< RCC PLL1 Dividers Configuration Register,                                Address offset: 0x30  */
    __IO uint32_t PLL1FRACR;      /*!< RCC PLL1 Fractional Divider Configuration Register,                      Address offset: 0x34  */
    __IO uint32_t PLL2DIVR;       /*!< RCC PLL2 Dividers Configuration Register,                                Address offset: 0x38  */
    __IO uint32_t PLL2FRACR;      /*!< RCC PLL2 Fractional Divider Configuration Register,                      Address offset: 0x3C  */
    __IO uint32_t PLL3DIVR;       /*!< RCC PLL3 Dividers Configuration Register,                                Address offset: 0x40  */
    __IO uint32_t PLL3FRACR;      /*!< RCC PLL3 Fractional Divider Configuration Register,                      Address offset: 0x44  */
    uint32_t      RESERVED3;      /*!< Reserved,                                                                Address offset: 0x48  */
    __IO uint32_t  D1CCIPR;       /*!< RCC Domain 1 Kernel Clock Configuration Register                         Address offset: 0x4C  */
    __IO uint32_t  D2CCIP1R;      /*!< RCC Domain 2 Kernel Clock Configuration Register                         Address offset: 0x50  */
    __IO uint32_t  D2CCIP2R;      /*!< RCC Domain 2 Kernel Clock Configuration Register                         Address offset: 0x54  */
    __IO uint32_t  D3CCIPR;       /*!< RCC Domain 3 Kernel Clock Configuration Register                         Address offset: 0x58  */
    uint32_t      RESERVED4;      /*!< Reserved,                                                                Address offset: 0x5C  */
    __IO uint32_t  CIER;          /*!< RCC Clock Source Interrupt Enable Register                               Address offset: 0x60  */
    __IO uint32_t  CIFR;          /*!< RCC Clock Source Interrupt Flag Register                                 Address offset: 0x64  */
    __IO uint32_t  CICR;          /*!< RCC Clock Source Interrupt Clear Register                                Address offset: 0x68  */
    uint32_t     RESERVED5;       /*!< Reserved,                                                                Address offset: 0x6C  */
    __IO uint32_t  BDCR;          /*!< RCC Vswitch Backup Domain Control Register,                              Address offset: 0x70  */
    __IO uint32_t  CSR;           /*!< RCC clock control & status register,                                     Address offset: 0x74  */
    uint32_t     RESERVED6;       /*!< Reserved,                                                                Address offset: 0x78  */
    __IO uint32_t AHB3RSTR;       /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x7C  */
    __IO uint32_t AHB1RSTR;       /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x80  */
    __IO uint32_t AHB2RSTR;       /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x84  */
    __IO uint32_t AHB4RSTR;       /*!< RCC AHB4 peripheral reset register,                                      Address offset: 0x88  */
    __IO uint32_t APB3RSTR;       /*!< RCC APB3 peripheral reset register,                                      Address offset: 0x8C  */
    __IO uint32_t APB1LRSTR;      /*!< RCC APB1 peripheral reset Low Word register,                             Address offset: 0x90  */
    __IO uint32_t APB1HRSTR;      /*!< RCC APB1 peripheral reset High Word register,                            Address offset: 0x94  */
    __IO uint32_t APB2RSTR;       /*!< RCC APB2 peripheral reset register,                                      Address offset: 0x98  */
    __IO uint32_t APB4RSTR;       /*!< RCC APB4 peripheral reset register,                                      Address offset: 0x9C  */
    __IO uint32_t GCR;            /*!< RCC RCC Global Control  Register,                                        Address offset: 0xA0  */
    uint32_t     RESERVED8;       /*!< Reserved,                                                                Address offset: 0xA4  */
    __IO uint32_t D3AMR;          /*!< RCC Domain 3 Autonomous Mode Register,                                   Address offset: 0xA8  */
    uint32_t     RESERVED11[9];    /*!< Reserved, 0xAC-0xCC                                                      Address offset: 0xAC  */
    __IO uint32_t RSR;            /*!< RCC Reset status register,                                               Address offset: 0xD0  */
    __IO uint32_t AHB3ENR;        /*!< RCC AHB3 peripheral clock  register,                                     Address offset: 0xD4  */
    __IO uint32_t AHB1ENR;        /*!< RCC AHB1 peripheral clock  register,                                     Address offset: 0xD8  */
    __IO uint32_t AHB2ENR;        /*!< RCC AHB2 peripheral clock  register,                                     Address offset: 0xDC  */
    __IO uint32_t AHB4ENR;        /*!< RCC AHB4 peripheral clock  register,                                     Address offset: 0xE0  */
    __IO uint32_t APB3ENR;        /*!< RCC APB3 peripheral clock  register,                                     Address offset: 0xE4  */
    __IO uint32_t APB1LENR;       /*!< RCC APB1 peripheral clock  Low Word register,                            Address offset: 0xE8  */
    __IO uint32_t APB1HENR;       /*!< RCC APB1 peripheral clock  High Word register,                           Address offset: 0xEC  */
    __IO uint32_t APB2ENR;        /*!< RCC APB2 peripheral clock  register,                                     Address offset: 0xF0  */
    __IO uint32_t APB4ENR;        /*!< RCC APB4 peripheral clock  register,                                     Address offset: 0xF4  */
    uint32_t      RESERVED12;      /*!< Reserved,                                                                Address offset: 0xF8  */
    __IO uint32_t AHB3LPENR;      /*!< RCC AHB3 peripheral sleep clock  register,                               Address offset: 0xFC  */
    __IO uint32_t AHB1LPENR;      /*!< RCC AHB1 peripheral sleep clock  register,                               Address offset: 0x100 */
    __IO uint32_t AHB2LPENR;      /*!< RCC AHB2 peripheral sleep clock  register,                               Address offset: 0x104 */
    __IO uint32_t AHB4LPENR;      /*!< RCC AHB4 peripheral sleep clock  register,                               Address offset: 0x108 */
    __IO uint32_t APB3LPENR;      /*!< RCC APB3 peripheral sleep clock  register,                               Address offset: 0x10C */
    __IO uint32_t APB1LLPENR;     /*!< RCC APB1 peripheral sleep clock  Low Word register,                      Address offset: 0x110 */
    __IO uint32_t APB1HLPENR;     /*!< RCC APB1 peripheral sleep clock  High Word register,                     Address offset: 0x114 */
    __IO uint32_t APB2LPENR;      /*!< RCC APB2 peripheral sleep clock  register,                               Address offset: 0x118 */
    __IO uint32_t APB4LPENR;      /*!< RCC APB4 peripheral sleep clock  register,                               Address offset: 0x11C */
    uint32_t     RESERVED13[4];   /*!< Reserved, 0x120-0x12C                                                    Address offset: 0x120 */
} RCC_TypeDef;


/* Gpio_TypeDef */
typedef struct {
    __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    __IO uint32_t BSRR;     /*!< GPIO port bit set/reset,               Address offset: 0x18      */
    __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;


/* TIM_TypeDef */
typedef struct {
    __IO uint32_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
    __IO uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
    __IO uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
    __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
    __IO uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
    __IO uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
    __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
    __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
    __IO uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
    __IO uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
    __IO uint32_t PSC;         /*!< TIM prescaler,                            Address offset: 0x28 */
    __IO uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
    __IO uint32_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
    __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
    __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
    __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
    __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
    __IO uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
    __IO uint32_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
    __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
    uint32_t      RESERVED1;   /*!< Reserved, 0x50                                                 */
    __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
    __IO uint32_t CCR5;        /*!< TIM capture/compare register5,            Address offset: 0x58 */
    __IO uint32_t CCR6;        /*!< TIM capture/compare register6,            Address offset: 0x5C */
    __IO uint32_t AF1;         /*!< TIM alternate function option register 1, Address offset: 0x60 */
    __IO uint32_t AF2;         /*!< TIM alternate function option register 2, Address offset: 0x64 */
    __IO uint32_t TISEL;       /*!< TIM Input Selection register,             Address offset: 0x68 */
} TIM_TypeDef;


/* DMA_TypeDef */
typedef struct {
    __IO uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
    __IO uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
    __IO uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
    __IO uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_TypeDef;

/* DMA_Stream_TypeDef */
typedef struct {
  __IO uint32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

/* DMAMAX_Channel_Typdef*/
typedef struct {
  __IO uint32_t  CCR;        /*!< DMA Multiplexer Channel x Control Register   */
}DMAMUX_Channel_TypeDef;

/* USART_TypeDef */
typedef struct {
    __IO uint32_t CR1;    /*!< USART Control register 1,                 Address offset: 0x00 */
    __IO uint32_t CR2;    /*!< USART Control register 2,                 Address offset: 0x04 */
    __IO uint32_t CR3;    /*!< USART Control register 3,                 Address offset: 0x08 */
    __IO uint32_t BRR;    /*!< USART Baud rate register,                 Address offset: 0x0C */
    __IO uint32_t GTPR;   /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
    __IO uint32_t RTOR;   /*!< USART Receiver Time Out register,         Address offset: 0x14 */
    __IO uint32_t RQR;    /*!< USART Request register,                   Address offset: 0x18 */
    __IO uint32_t ISR;    /*!< USART Interrupt and status register,      Address offset: 0x1C */
    __IO uint32_t ICR;    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
    __IO uint32_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
    __IO uint32_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
    __IO uint32_t PRESC;  /*!< USART clock Prescaler register,           Address offset: 0x2C */
} USART_TypeDef;



/* I2C_Typdef */
typedef struct {
    __IO uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
    __IO uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
    __IO uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
    __IO uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
    __IO uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
    __IO uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
    __IO uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
    __IO uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
    __IO uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
    __IO uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
    __IO uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_TypeDef;


/* EXTI_TypeDef */
typedef struct {
    __IO uint32_t RTSR1;               /*!< EXTI Rising trigger selection register,          Address offset: 0x00 */
    __IO uint32_t FTSR1;               /*!< EXTI Falling trigger selection register,         Address offset: 0x04 */
    __IO uint32_t SWIER1;              /*!< EXTI Software interrupt event register,          Address offset: 0x08 */
    __IO uint32_t D3PMR1;              /*!< EXTI D3 Pending mask register, (same register as to SRDPMR1) Address offset: 0x0C */
    __IO uint32_t D3PCR1L;             /*!< EXTI D3 Pending clear selection register low, (same register as to SRDPCR1L)     Address offset: 0x10 */
    __IO uint32_t D3PCR1H;             /*!< EXTI D3 Pending clear selection register High, (same register as to SRDPCR1H)   Address offset: 0x14 */
    uint32_t      RESERVED1[2];        /*!< Reserved,                                        0x18 to 0x1C         */
    __IO uint32_t RTSR2;               /*!< EXTI Rising trigger selection register,          Address offset: 0x20 */
    __IO uint32_t FTSR2;               /*!< EXTI Falling trigger selection register,         Address offset: 0x24 */
    __IO uint32_t SWIER2;              /*!< EXTI Software interrupt event register,          Address offset: 0x28 */
    __IO uint32_t D3PMR2;              /*!< EXTI D3 Pending mask register, (same register as to SRDPMR2) Address offset: 0x2C */
    __IO uint32_t D3PCR2L;             /*!< EXTI D3 Pending clear selection register low, (same register as to SRDPCR2L)  Address offset: 0x30 */
    __IO uint32_t D3PCR2H;             /*!< EXTI D3 Pending clear selection register High, (same register as to SRDPCR2H) Address offset: 0x34 */
    uint32_t      RESERVED2[2];        /*!< Reserved,                                        0x38 to 0x3C         */
    __IO uint32_t RTSR3;               /*!< EXTI Rising trigger selection register,          Address offset: 0x40 */
    __IO uint32_t FTSR3;               /*!< EXTI Falling trigger selection register,         Address offset: 0x44 */
    __IO uint32_t SWIER3;              /*!< EXTI Software interrupt event register,          Address offset: 0x48 */
    __IO uint32_t D3PMR3;              /*!< EXTI D3 Pending mask register, (same register as to SRDPMR3) Address offset: 0x4C */
    __IO uint32_t D3PCR3L;             /*!< EXTI D3 Pending clear selection register low, (same register as to SRDPCR3L) Address offset: 0x50 */
    __IO uint32_t D3PCR3H;             /*!< EXTI D3 Pending clear selection register High, (same register as to SRDPCR3H) Address offset: 0x54 */
    uint32_t      RESERVED3[10];       /*!< Reserved,                                        0x58 to 0x7C         */
    __IO uint32_t IMR1;                /*!< EXTI Interrupt mask register,                    Address offset: 0x80 */
    __IO uint32_t EMR1;                /*!< EXTI Event mask register,                        Address offset: 0x84 */
    __IO uint32_t PR1;                 /*!< EXTI Pending register,                           Address offset: 0x88 */
    uint32_t      RESERVED4;           /*!< Reserved,                                        0x8C                 */
    __IO uint32_t IMR2;                /*!< EXTI Interrupt mask register,                    Address offset: 0x90 */
    __IO uint32_t EMR2;                /*!< EXTI Event mask register,                        Address offset: 0x94 */
    __IO uint32_t PR2;                 /*!< EXTI Pending register,                           Address offset: 0x98 */
    uint32_t      RESERVED5;           /*!< Reserved,                                        0x9C                 */
    __IO uint32_t IMR3;                /*!< EXTI Interrupt mask register,                    Address offset: 0xA0 */
    __IO uint32_t EMR3;                /*!< EXTI Event mask register,                        Address offset: 0xA4 */
    __IO uint32_t PR3;                 /*!< EXTI Pending register,                           Address offset: 0xA8 */

}EXTI_TypeDef;


/* enum for IRQ number */
typedef enum {
    /******  Cortex-M Processor Exceptions Numbers *****************************************************************/
    NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
    HardFault_IRQn              = -13,    /*!< 3 Cortex-M Hard Fault Interrupt                                   */
    MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M Memory Management Interrupt                            */
    BusFault_IRQn               = -11,    /*!< 5 Cortex-M Bus Fault Interrupt                                    */
    UsageFault_IRQn             = -10,    /*!< 6 Cortex-M Usage Fault Interrupt                                  */
    SVCall_IRQn                 = -5,     /*!< 11 Cortex-M SV Call Interrupt                                     */
    DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M Debug Monitor Interrupt                               */
    PendSV_IRQn                 = -2,     /*!< 14 Cortex-M Pend SV Interrupt                                     */
    SysTick_IRQn                = -1,     /*!< 15 Cortex-M System Tick Interrupt                                 */
    /******  STM32 specific Interrupt Numbers **********************************************************************/
    WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt ( wwdg1_it, wwdg2_it)                   */
    PVD_AVD_IRQn                = 1,      /*!< PVD/AVD through EXTI Line detection Interrupt                     */
    TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
    RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
    FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
    RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
    EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
    EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
    EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
    EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
    EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
    DMA1_Stream0_IRQn           = 11,     /*!< DMA1 Stream 0 global Interrupt                                    */
    DMA1_Stream1_IRQn           = 12,     /*!< DMA1 Stream 1 global Interrupt                                    */
    DMA1_Stream2_IRQn           = 13,     /*!< DMA1 Stream 2 global Interrupt                                    */
    DMA1_Stream3_IRQn           = 14,     /*!< DMA1 Stream 3 global Interrupt                                    */
    DMA1_Stream4_IRQn           = 15,     /*!< DMA1 Stream 4 global Interrupt                                    */
    DMA1_Stream5_IRQn           = 16,     /*!< DMA1 Stream 5 global Interrupt                                    */
    DMA1_Stream6_IRQn           = 17,     /*!< DMA1 Stream 6 global Interrupt                                    */
    ADC_IRQn                    = 18,     /*!< ADC1 and  ADC2 global Interrupts                                  */
    FDCAN1_IT0_IRQn             = 19,     /*!< FDCAN1 Interrupt line 0                                           */
    FDCAN2_IT0_IRQn             = 20,     /*!< FDCAN2 Interrupt line 0                                           */
    FDCAN1_IT1_IRQn             = 21,     /*!< FDCAN1 Interrupt line 1                                           */
    FDCAN2_IT1_IRQn             = 22,     /*!< FDCAN2 Interrupt line 1                                           */
    EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
    TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                              */
    TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                             */
    TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt                            */
    TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
    TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
    TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
    TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
    I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
    I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
    I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
    I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
    SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
    SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
    USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
    USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
    USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
    EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
    RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
    TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
    TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
    TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
    TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
    DMA1_Stream7_IRQn           = 47,     /*!< DMA1 Stream7 Interrupt                                            */
    FMC_IRQn                    = 48,     /*!< FMC global Interrupt                                              */
    SDMMC1_IRQn                 = 49,     /*!< SDMMC1 global Interrupt                                           */
    TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
    SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
    UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
    UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
    TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
    TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
    DMA2_Stream0_IRQn           = 56,     /*!<   DMA2 Stream 0 global Interrupt                                  */
    DMA2_Stream1_IRQn           = 57,     /*!<   DMA2 Stream 1 global Interrupt                                  */
    DMA2_Stream2_IRQn           = 58,     /*!<   DMA2 Stream 2 global Interrupt                                  */
    DMA2_Stream3_IRQn           = 59,     /*!<   DMA2 Stream 3 global Interrupt                                  */
    DMA2_Stream4_IRQn           = 60,     /*!<   DMA2 Stream 4 global Interrupt                                  */
    ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                                         */
    ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
    FDCAN_CAL_IRQn              = 63,     /*!< FDCAN Calibration unit Interrupt                                  */
    DMA2_Stream5_IRQn           = 68,     /*!< DMA2 Stream 5 global interrupt                                    */
    DMA2_Stream6_IRQn           = 69,     /*!< DMA2 Stream 6 global interrupt                                    */
    DMA2_Stream7_IRQn           = 70,     /*!< DMA2 Stream 7 global interrupt                                    */
    USART6_IRQn                 = 71,     /*!< USART6 global interrupt                                           */
    I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
    I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
    OTG_HS_EP1_OUT_IRQn         = 74,     /*!< USB OTG HS End Point 1 Out global interrupt                       */
    OTG_HS_EP1_IN_IRQn          = 75,     /*!< USB OTG HS End Point 1 In global interrupt                        */
    OTG_HS_WKUP_IRQn            = 76,     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
    OTG_HS_IRQn                 = 77,     /*!< USB OTG HS global interrupt                                       */
    DCMI_IRQn                   = 78,     /*!< DCMI global interrupt                                             */
    CRYP_IRQn                   = 79,     /*!< CRYP crypto global interrupt                                      */
    HASH_RNG_IRQn               = 80,     /*!< HASH and RNG global interrupt                                     */
    FPU_IRQn                    = 81,     /*!< FPU global interrupt                                              */
    UART7_IRQn                  = 82,     /*!< UART7 global interrupt                                            */
    UART8_IRQn                  = 83,     /*!< UART8 global interrupt                                            */
    SPI4_IRQn                   = 84,     /*!< SPI4 global Interrupt                                             */
    SPI5_IRQn                   = 85,     /*!< SPI5 global Interrupt                                             */
    SPI6_IRQn                   = 86,     /*!< SPI6 global Interrupt                                             */
    SAI1_IRQn                   = 87,     /*!< SAI1 global Interrupt                                             */
    LTDC_IRQn                   = 88,     /*!< LTDC global Interrupt                                             */
    LTDC_ER_IRQn                = 89,     /*!< LTDC Error global Interrupt                                       */
    DMA2D_IRQn                  = 90,     /*!< DMA2D global Interrupt                                            */
    SAI2_IRQn                   = 91,     /*!< SAI2 global Interrupt                                             */
    QUADSPI_IRQn                = 92,     /*!< Quad SPI global interrupt                                         */
    LPTIM1_IRQn                 = 93,     /*!< LP TIM1 interrupt                                                 */
    CEC_IRQn                    = 94,     /*!< HDMI-CEC global Interrupt                                         */
    I2C4_EV_IRQn                = 95,     /*!< I2C4 Event Interrupt                                              */
    I2C4_ER_IRQn                = 96,     /*!< I2C4 Error Interrupt                                              */
    SPDIF_RX_IRQn               = 97,     /*!< SPDIF-RX global Interrupt                                         */
    OTG_FS_EP1_OUT_IRQn         = 98,     /*!< USB OTG HS2 global interrupt                                      */
    OTG_FS_EP1_IN_IRQn          = 99,     /*!< USB OTG HS2 End Point 1 Out global interrupt                      */
    OTG_FS_WKUP_IRQn            = 100,    /*!< USB OTG HS2 End Point 1 In global interrupt                       */
    OTG_FS_IRQn                 = 101,    /*!< USB OTG HS2 Wakeup through EXTI interrupt                         */
    DMAMUX1_OVR_IRQn            = 102,    /*!<DMAMUX1 Overrun interrupt                                          */
    HRTIM1_Master_IRQn          = 103,    /*!< HRTIM Master Timer global Interrupts                              */
    HRTIM1_TIMA_IRQn            = 104,    /*!< HRTIM Timer A global Interrupt                                    */
    HRTIM1_TIMB_IRQn            = 105,    /*!< HRTIM Timer B global Interrupt                                    */
    HRTIM1_TIMC_IRQn            = 106,    /*!< HRTIM Timer C global Interrupt                                    */
    HRTIM1_TIMD_IRQn            = 107,    /*!< HRTIM Timer D global Interrupt                                    */
    HRTIM1_TIME_IRQn            = 108,    /*!< HRTIM Timer E global Interrupt                                    */
    HRTIM1_FLT_IRQn             = 109,    /*!< HRTIM Fault global Interrupt                                      */
    DFSDM1_FLT0_IRQn            = 110,    /*!<DFSDM Filter1 Interrupt                                            */
    DFSDM1_FLT1_IRQn            = 111,    /*!<DFSDM Filter2 Interrupt                                            */
    DFSDM1_FLT2_IRQn            = 112,    /*!<DFSDM Filter3 Interrupt                                            */
    DFSDM1_FLT3_IRQn            = 113,    /*!<DFSDM Filter4 Interrupt                                            */
    SAI3_IRQn                   = 114,    /*!< SAI3 global Interrupt                                             */
    SWPMI1_IRQn                 = 115,    /*!< Serial Wire Interface 1 global interrupt                          */
    TIM15_IRQn                  = 116,    /*!< TIM15 global Interrupt                                            */
    TIM16_IRQn                  = 117,    /*!< TIM16 global Interrupt                                            */
    TIM17_IRQn                  = 118,    /*!< TIM17 global Interrupt                                            */
    MDIOS_WKUP_IRQn             = 119,    /*!< MDIOS Wakeup  Interrupt                                           */
    MDIOS_IRQn                  = 120,    /*!< MDIOS global Interrupt                                            */
    JPEG_IRQn                   = 121,    /*!< JPEG global Interrupt                                             */
    MDMA_IRQn                   = 122,    /*!< MDMA global Interrupt                                             */
    SDMMC2_IRQn                 = 124,    /*!< SDMMC2 global Interrupt                                           */
    HSEM1_IRQn                  = 125,    /*!< HSEM1 global Interrupt                                            */
    ADC3_IRQn                   = 127,    /*!< ADC3 global Interrupt                                             */
    DMAMUX2_OVR_IRQn            = 128,    /*!<DMAMUX2 Overrun interrupt                                          */
    BDMA_Channel0_IRQn          = 129,    /*!< BDMA Channel 0 global Interrupt                                   */
    BDMA_Channel1_IRQn          = 130,    /*!< BDMA Channel 1 global Interrupt                                   */
    BDMA_Channel2_IRQn          = 131,    /*!< BDMA Channel 2 global Interrupt                                   */
    BDMA_Channel3_IRQn          = 132,    /*!< BDMA Channel 3 global Interrupt                                   */
    BDMA_Channel4_IRQn          = 133,    /*!< BDMA Channel 4 global Interrupt                                   */
    BDMA_Channel5_IRQn          = 134,    /*!< BDMA Channel 5 global Interrupt                                   */
    BDMA_Channel6_IRQn          = 135,    /*!< BDMA Channel 6 global Interrupt                                   */
    BDMA_Channel7_IRQn          = 136,    /*!< BDMA Channel 7 global Interrupt                                   */
    COMP_IRQn                   = 137 ,   /*!< COMP global Interrupt                                             */
    LPTIM2_IRQn                 = 138,    /*!< LP TIM2 global interrupt                                          */
    LPTIM3_IRQn                 = 139,    /*!< LP TIM3 global interrupt                                          */
    LPTIM4_IRQn                 = 140,    /*!< LP TIM4 global interrupt                                          */
    LPTIM5_IRQn                 = 141,    /*!< LP TIM5 global interrupt                                          */
    LPUART1_IRQn                = 142,    /*!< LP UART1 interrupt                                                */
    CRS_IRQn                    = 144,    /*!< Clock Recovery Global Interrupt                                   */
    ECC_IRQn                    = 145,    /*!< ECC diagnostic Global Interrupt                                   */
    SAI4_IRQn                   = 146,    /*!< SAI4 global interrupt                                             */
    WAKEUP_PIN_IRQn             = 149,    /*!< Interrupt for all 6 wake-up pins                                  */
} IRQn_Type;


/* look in mem map in ref */
/*
                        Base address define
*/
/* base address of RCC */
#define RCC_BASE                    (0x58024400UL)      

/* base address of PWR */
#define PWR_BASE                    (0x58024800UL)


/* base address of FLASH*/
#define FLASH_BASE                  (0x52002000UL)

/* base address of EXTI */
#define EXTI_BASE                   (0x58000000UL)

/* base address off GPIO */
#define GPIOK_BASE                  (0x58022800UL)
#define GPIOJ_BASE                  (0x58022400UL)
#define GPIOI_BASE                  (0x58022000UL)
#define GPIOH_BASE                  (0x58021C00UL)
#define GPIOG_BASE                  (0x58021800UL)
#define GPIOF_BASE                  (0x58021400UL)
#define GPIOE_BASE                  (0x58021000UL)
#define GPIOD_BASE                  (0x58020C00UL)
#define GPIOC_BASE                  (0x58020800UL)
#define GPIOB_BASE                  (0x58020400UL)
#define GPIOA_BASE                  (0x58020000UL)

/* base address of timer */
#define TIM14_BASE                  (0x40002000UL)
#define TIM13_BASE                  (0x40001C00UL)
#define TIM12_BASE                  (0x40001800UL)
#define TIM7_BASE                   (0x40001400UL)
#define TIM6_BASE                   (0x40001000UL)
#define TIM5_BASE                   (0x40000C00UL)
#define TIM4_BASE                   (0x40000800UL)
#define TIM3_BASE                   (0x40000400UL)
#define TIM2_BASE                   (0x40000000UL)


/* base address of advance timer */
#define TIM8_BASE                   (0x40010400UL)
#define TIM1_BASE                   (0x40010000UL)

/* base address of dma */
#define DMA2_BASE                   (0x40020400UL)
#define DMA1_BASE                   (0x40020000UL)

#define DMA1_Stream0_BASE     (DMA1_BASE + 0x010UL)
#define DMA1_Stream1_BASE     (DMA1_BASE + 0x028UL)
#define DMA1_Stream2_BASE     (DMA1_BASE + 0x040UL)
#define DMA1_Stream3_BASE     (DMA1_BASE + 0x058UL)
#define DMA1_Stream4_BASE     (DMA1_BASE + 0x070UL)
#define DMA1_Stream5_BASE     (DMA1_BASE + 0x088UL)
#define DMA1_Stream6_BASE     (DMA1_BASE + 0x0A0UL)
#define DMA1_Stream7_BASE     (DMA1_BASE + 0x0B8UL)

#define DMA2_Stream0_BASE     (DMA2_BASE + 0x010UL)
#define DMA2_Stream1_BASE     (DMA2_BASE + 0x028UL)
#define DMA2_Stream2_BASE     (DMA2_BASE + 0x040UL)
#define DMA2_Stream3_BASE     (DMA2_BASE + 0x058UL)
#define DMA2_Stream4_BASE     (DMA2_BASE + 0x070UL)
#define DMA2_Stream5_BASE     (DMA2_BASE + 0x088UL)
#define DMA2_Stream6_BASE     (DMA2_BASE + 0x0A0UL)
#define DMA2_Stream7_BASE     (DMA2_BASE + 0x0B8UL)


/* base address of DMAMUX1 */
#define DMAMUX1_BASE             (0x40020800UL)
#define DMAMUX1_Channel0_BASE    (DMAMUX1_BASE)
#define DMAMUX1_Channel1_BASE    (DMAMUX1_BASE + 0x0004UL)
#define DMAMUX1_Channel2_BASE    (DMAMUX1_BASE + 0x0008UL)
#define DMAMUX1_Channel3_BASE    (DMAMUX1_BASE + 0x000CUL)
#define DMAMUX1_Channel4_BASE    (DMAMUX1_BASE + 0x0010UL)
#define DMAMUX1_Channel5_BASE    (DMAMUX1_BASE + 0x0014UL)
#define DMAMUX1_Channel6_BASE    (DMAMUX1_BASE + 0x0018UL)
#define DMAMUX1_Channel7_BASE    (DMAMUX1_BASE + 0x001CUL)
#define DMAMUX1_Channel8_BASE    (DMAMUX1_BASE + 0x0020UL)
#define DMAMUX1_Channel9_BASE    (DMAMUX1_BASE + 0x0024UL)
#define DMAMUX1_Channel10_BASE   (DMAMUX1_BASE + 0x0028UL)
#define DMAMUX1_Channel11_BASE   (DMAMUX1_BASE + 0x002CUL)
#define DMAMUX1_Channel12_BASE   (DMAMUX1_BASE + 0x0030UL)
#define DMAMUX1_Channel13_BASE   (DMAMUX1_BASE + 0x0034UL)
#define DMAMUX1_Channel14_BASE   (DMAMUX1_BASE + 0x0038UL)
#define DMAMUX1_Channel15_BASE   (DMAMUX1_BASE + 0x003CUL)



/* base address of uasrt*/
#define USART6_BASE                 (0x40011400UL)
#define USART1_BASE                 (0x40011000UL)
#define USART3_BASE                 (0x40004800UL)
#define USART2_BASE                 (0x40004400UL)

/* base address of i2c */
#define I2C1_BASE                   (0x40005400UL)
#define I2C2_BASE                   (0x40005800UL)
#define I2C3_BASE                   (0x40005C00UL)
#define I2C4_BASE                   (0x58001C00UL)




/*
                        Define reg for use
*/

/* reg for RCC*/
#define RCC                         ((RCC_TypeDef*) RCC_BASE)

/* reg for PWR */
#define PWR                         ((PWR_TypeDef*) PWR_BASE)

/* reg for GPIO */
#define GPIOK                       ((GPIO_TypeDef*) GPIOK_BASE)
#define GPIOJ                       ((GPIO_TypeDef*) GPIOJ_BASE)
#define GPIOI                       ((GPIO_TypeDef*) GPIOI_BASE)
#define GPIOH                       ((GPIO_TypeDef*) GPIOH_BASE)
#define GPIOG                       ((GPIO_TypeDef*) GPIOG_BASE)
#define GPIOF                       ((GPIO_TypeDef*) GPIOF_BASE)
#define GPIOE                       ((GPIO_TypeDef*) GPIOE_BASE)
#define GPIOD                       ((GPIO_TypeDef*) GPIOD_BASE)
#define GPIOC                       ((GPIO_TypeDef*) GPIOC_BASE)
#define GPIOB                       ((GPIO_TypeDef*) GPIOB_BASE)
#define GPIOA                       ((GPIO_TypeDef*) GPIOA_BASE)

/* reg for timer */
#define TIM14                       ((TIM_TypeDef*) TIM14_BASE)
#define TIM13                       ((TIM_TypeDef*) TIM13_BASE)
#define TIM12                       ((TIM_TypeDef*) TIM12_BASE)
#define TIM7                        ((TIM_TypeDef*) TIM7_BASE)
#define TIM6                        ((TIM_TypeDef*) TIM6_BASE)
#define TIM5                        ((TIM_TypeDef*) TIM5_BASE)
#define TIM4                        ((TIM_TypeDef*) TIM4_BASE)
#define TIM3                        ((TIM_TypeDef*) TIM3_BASE)
#define TIM2                        ((TIM_TypeDef*) TIM2_BASE)

/* reg for advance timer */
#define TIM1                        ((TIM_TypeDef*) TIM1_BASE)
#define TIM8                        ((TIM_TypeDef*) TIM8_BASE)

/* reg for dma */
#define DMA2                        ((DMA_TypeDef*) DMA2_BASE)
#define DMA1                        ((DMA_TypeDef*) DMA1_BASE)

#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)

#define DMA2_Stream0        ((DMA_Stream_TypeDef *) DMA2_Stream0_BASE)
#define DMA2_Stream1        ((DMA_Stream_TypeDef *) DMA2_Stream1_BASE)
#define DMA2_Stream2        ((DMA_Stream_TypeDef *) DMA2_Stream2_BASE)
#define DMA2_Stream3        ((DMA_Stream_TypeDef *) DMA2_Stream3_BASE)
#define DMA2_Stream4        ((DMA_Stream_TypeDef *) DMA2_Stream4_BASE)
#define DMA2_Stream5        ((DMA_Stream_TypeDef *) DMA2_Stream5_BASE)
#define DMA2_Stream6        ((DMA_Stream_TypeDef *) DMA2_Stream6_BASE)
#define DMA2_Stream7        ((DMA_Stream_TypeDef *) DMA2_Stream7_BASE)


/* DMA_MUX1*/
#define DMAMUX1              ((DMAMUX_Channel_TypeDef *) DMAMUX1_BASE)
#define DMAMUX1_Channel0     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel0_BASE)
#define DMAMUX1_Channel1     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel1_BASE)
#define DMAMUX1_Channel2     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel2_BASE)
#define DMAMUX1_Channel3     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel3_BASE)
#define DMAMUX1_Channel4     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel4_BASE)
#define DMAMUX1_Channel5     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel5_BASE)
#define DMAMUX1_Channel6     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel6_BASE)
#define DMAMUX1_Channel7     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel7_BASE)
#define DMAMUX1_Channel8     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel8_BASE)
#define DMAMUX1_Channel9     ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel9_BASE)
#define DMAMUX1_Channel10    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel10_BASE)
#define DMAMUX1_Channel11    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel11_BASE)
#define DMAMUX1_Channel12    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel12_BASE)
#define DMAMUX1_Channel13    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel13_BASE)
#define DMAMUX1_Channel14    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel14_BASE)
#define DMAMUX1_Channel15    ((DMAMUX_Channel_TypeDef *) DMAMUX1_Channel15_BASE)


/* reg for usart */
#define USART6                      ((USART_TypeDef*) USART6_BASE)
#define USART1                      ((USART_TypeDef*) USART1_BASE)
#define USART3                      ((USART_TypeDef*) USART3_BASE)
#define USART2                      ((USART_TypeDef*) USART2_BASE)


/* reg for i2c */
#define I2C1                        ((I2C_TypeDef*) I2C1_BASE)
#define I2C2                        ((I2C_TypeDef*) I2C2_BASE)
#define I2C3                        ((I2C_TypeDef*) I2C3_BASE)
#define I2C4                        ((I2C_TypeDef*) I2C4_BASE)


/* reg for flash */
#define FLASH                       ((FLASH_TypeDef*) FLASH_BASE)

/* reg for EXTI*/
#define EXTI                        ((EXTI_TypeDef*) EXTI_BASE)



#define __CM7_REV               0x0100U   /*!< Cortex-M7 revision r1p0                       */
#define __MPU_PRESENT             1U       /*!< CM7 provides an MPU                           */
#define __NVIC_PRIO_BITS          4U       /*!< CM7 uses 4 Bits for the Priority Levels       */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /*!< FPU present                                   */
#define __ICACHE_PRESENT          1U       /*!< CM7 instruction cache present                 */
#define __DCACHE_PRESENT          1U       /*!< CM7 data cache present                        */
#include "core_cm7.h"                     /*!< Cortex-M7 processor and core peripherals      */



#endif