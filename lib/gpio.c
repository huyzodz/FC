#include "gpio.h"
#include "stm32h750vbt6.h"

void gpio_init(gpio_config_t *handle)
{
    GPIO_TypeDef *p = (GPIO_TypeDef *)(GPIOA_BASE + handle->gpio*0x400UL);
    uint8_t pinNum = handle->pinNum;
    gpio_mode mode = handle->mode;
    alternate_mode ALmode = handle->alternate;
    gpio_output_speed speed = handle->OutSpeed;

    /* turn on clock use for gpio */
    RCC->AHB4ENR |= (1 << (((uint32_t)p - (uint32_t)GPIOA)/1024));


    if (mode == GPIO_ALTERNATE_INPUT || mode == GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN || mode == GPIO_ALTERNATE_OUTPUT_PUSH_PULL || mode == GPIO_ALTERNATE_INPUT_PULL_UP || mode == GPIO_ALTERNATE_INPUT_PULL_DOWN)
    {
        /* pointer to AFRL or AFRH */
        uint32_t *AF;
        /* calculate to shift */
        uint8_t shift = (pinNum <= 7) ? pinNum : (pinNum - 8);
        if (pinNum <= 7)
        {
            /* AFRL */
            AF = &p->AFR[0];
        }
        else 
        {
            /* AFRH */
            AF = &p->AFR[1];
        }
        
        /* reset mode */
        p->MODER &= ~(0x03 << (pinNum*2));
        /* chose mode */
        p->MODER |= (2 << (pinNum*2));
        /* reset */
        *AF &= ~(0x0F << (shift*4));
        /* apply alternate mode to reg */
        *AF |= (ALmode << (shift*4));    

        if (mode == GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN)
        {   
            /* set 1 to bit in reg OTYPER */
            p->OTYPER |= (1 << pinNum);
        }
        else if (mode == GPIO_ALTERNATE_OUTPUT_PUSH_PULL)
        {
            /* reset, default is 0 */
            p->OTYPER &= ~(1 << pinNum);   
        }
        else
        {
            /* set for input alternate */
            /* reset */
            p->PUPDR &= ~(0x03 << (pinNum*2));
            /* set pullup/pulldown*/
            p->PUPDR |= ((mode - GPIO_ALTERNATE_INPUT) << (pinNum*2));
        }
    }
    else
    {
        /* config for gpio normal*/
        if (mode - GPIO_INPUT <= 2)
        {
            /* set for input */
            p->MODER &= ~(0x03 << (pinNum*2));
            /* reset pin for input */
            p->PUPDR &= ~(0x03 << (pinNum*2));
            /* set if mode is pull up */
            if (mode == GPIO_INPUT_PULLUP)
                p->PUPDR |= (1 << (pinNum*2));
            /* set if mode is pull down */
            else if (mode == GPIO_INPUT_PULLDOWN)
                p->PUPDR |= (2 << (pinNum*2));
        }
        else
        {
            /* reset MODER */
            p->MODER &= ~(0x03 << (pinNum*2));
            /* set for output */
            p->MODER |= (1 << (pinNum*2));
            /* reset for normarl output push pull */
            p->OTYPER &= ~(1 << pinNum);
            /* set 1 if open drain */
            if (mode == GPIO_OUTPUT_OPEN_DRAIN)
                p->OTYPER |= (1 << pinNum); 
        }

        

    }


    /* set speed for output */
    if (mode == GPIO_OUTPUT_PULL_UP || mode == GPIO_OUTPUT_OPEN_DRAIN || mode == GPIO_ALTERNATE_OUTPUT_OPEN_DRAIN || mode == GPIO_ALTERNATE_OUTPUT_PUSH_PULL)
    {
        p->OSPEEDR &= ~(0x03 << (pinNum*2));
        p->OSPEEDR |= (speed << (pinNum*2));
    }
}

void gpio_write(gpio_port gpio, uint8_t pin, uint8_t level)
{
    GPIO_TypeDef *p = (GPIO_TypeDef *)(GPIOA_BASE + gpio*0x400UL);

    if (level == 0)
    {   
        /* reset BS */
        p->BSRR &= ~(1 << pin);
        /* set 1 to BR */
        p->BSRR |= (1 << (pin+16));
    }
    else
    {
        /* reset BR */
        p->BSRR &= ~(1 << (pin+16));
        /* set 1 to BS */
        p->BSRR |= (1 << pin);
    }
}

uint16_t gpio_read(gpio_port gpio, uint8_t pin)
{
    GPIO_TypeDef *p = (GPIO_TypeDef *)(GPIOA_BASE + gpio*0x400UL);
    return ((p->IDR >> pin) & 0x01);
}