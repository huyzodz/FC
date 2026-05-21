#include "stdio.h"
#include "stdint.h"

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

typedef struct {
    uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
    uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
    uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
    uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
    uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
    uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
    uint32_t BSRR;     /*!< GPIO port bit set/reset,               Address offset: 0x18      */
    uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
    uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;


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

int main()
{
    printf("%d", GPIOB-GPIOA);
    return 0;
}